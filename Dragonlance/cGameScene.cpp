#include "cGameScene.h"
#include "cMap.h"
#include "Parameters.h"
#include "cGraphics.h"
#include "cTimer.h"
#include "cItemFactory.h"
#include <iostream>

void cGameScene::draw() {
	currentMap->draw(renderer, screen);
	for (auto e : currentMap->getNPC()) {
		// std::cout << "Orc: " << e->worldY + screen.biasRow << " " << e->worldX + screen.biasCol << std::endl;
		// only draw entity if player can see the tile
		if (!(currentMap->getMap()[e->worldY / TILE_HEIGHT][e->worldX / TILE_WIDTH].shadowed)) {
			// entity must be on screen
			if (e->worldX + screen.biasCol >= 0 && e->worldX + screen.biasCol <= GAME_SCREEN_WIDTH - TILE_WIDTH) {
				if (e->worldY + screen.biasRow >= 0 && e->worldY + screen.biasRow <= GAME_SCREEN_HEIGHT - TILE_HEIGHT) {
					drawEntity(*e);
				}
			}
		}
	}
	drawEntity(playerEntity);
}

void cGameScene::drawPlayer() {
	renderer->RenderRect(playerEntity.worldX + screen.biasCol, playerEntity.worldY + screen.biasRow, TILE_WIDTH, TILE_HEIGHT,
		255, 0, 0, 255);
}

void cGameScene::drawEntity(const cEntity& e) {
	renderer->RenderEntityWithBias(e, screen.biasCol, screen.biasRow);
}

void cGameScene::updatePlayer(int dir) {
	switch (dir) {
	case EAST: {
		// Note that biases are extracted, so should be smaller when moving east
		if (playerEntity.worldX < (currentMap->getMapWidth() -1) * TILE_WIDTH) {
			playerEntity.worldX += TILE_WIDTH;
		}
		break;
	}
	case SOUTH: {
		std::cout << playerEntity.worldY << " vs " << (currentMap->getMapHeight() - 1) * TILE_HEIGHT << std::endl;
		if (playerEntity.worldY < (currentMap->getMapHeight() -1) * TILE_HEIGHT) {
			playerEntity.worldY += TILE_HEIGHT;
		}
		break;
	}
	case WEST: {
		if (playerEntity.worldX > 0) {
			playerEntity.worldX -= TILE_WIDTH;
		}
		break;
	}
	case NORTH: {
		if (playerEntity.worldY > 0) {
			playerEntity.worldY -= TILE_HEIGHT;
		}
		break;
	}
	default: {
		break;
	}
	}
	// std::cout << playerEntity.worldX << " " << playerEntity.worldY << std::endl;
	// screen.player = player;
	this->shadowCastingExperimentalFOV();
}

void cGameScene::registerNPC(cEntity* e, std::string mapID, cMapLocation l) {
	SDL_assert(e != nullptr);
	SDL_assert((*map).find(mapID) != (*map).end());
	SDL_assert(l.x >= 0 && l.y >= 0);
	SDL_assert(l.x <= (*map)[mapID]->getMapWidth() * TILE_WIDTH - TILE_WIDTH);
	SDL_assert(l.x <= (*map)[mapID]->getMapHeight() * TILE_HEIGHT - TILE_HEIGHT);
	// Use custom location
	e->worldX = l.x;
	e->worldY = l.y;
	(*map)[mapID]->addNPC(e);
}

void cGameScene::shadowCastingEnhancedFOV() {
	/*
	* FOV: proper shadow casting 
	* 8 octants based on: http://www.roguebasin.com/index.php/FOV_using_recursive_shadowcasting
	* The tricky part is player is not always at the center of the screen
	*/

	// Remember i is for X which is horizontal and j is for Y which is vertical
	// TODO: We also completely ignore a part if player is too close to it
	// TODO: For example, too close to bottom then we only check upper part

	int minCol = std::max((screen.topLeftViewport.x - screen.biasCol) / TILE_WIDTH, 0);
	int minRow = std::max((screen.topLeftViewport.y - screen.biasRow) / TILE_HEIGHT, 0);
	int maxCol = std::min(minCol + screen.topLeftViewport.w / TILE_WIDTH, currentMap->getMapWidth());
	int maxRow = std::min(minRow + screen.topLeftViewport.h / TILE_HEIGHT, currentMap->getMapHeight());

	int playerCol = playerEntity.worldX / TILE_WIDTH;	// Pixel to index
	int playerRow = playerEntity.worldY / TILE_HEIGHT;

	std::cout << minRow << std::endl;

	// std::cout << "(" << playerCol << ", " << playerRow << ")";

	// Clean all shadows
	for (int col = minCol; col < maxCol; col++) {
		for (int row = minRow; row < maxRow; row++) {
			currentMap->getMap()[row][col].shadowed = false;
		}
	}

	int shadowed = 0;

	/* Octant 2 (includes playerCol)
	* Loop explanation:
	* - For the inner loop, the std::min() is needed if player is in right half of the map
	* - i is col and j is row
	* - Should exclude 45 degree but incude 90 degree
	*/
	for (int row = playerRow - 1; row > minRow; row--) {
		for (int col = playerCol; col < std::min(maxCol, playerCol + playerRow - row); col++) {
			if (currentMap->getMap()[row][col].blocker == true) {
				// TODO: Loop to find last tile of a continous block of blocker tiles
				// TODO: Then we only cast two rays for the whole block
				// TODO: Also use two points that create largest amount of shadow
				// TODO: otherwise we will have gaps between two octants

				int startCol = col;
				// Note that col eventually points to a non-blocker,
				// so the col++ in the for loop skips [row][col] as a bonus
				while (currentMap->getMap()[row][col].blocker == true) {
					col++;
					if (col == std::min(maxCol, playerCol + playerRow - row)) {
						break;
					}
				}
				int endCol = col - 1;
				/*
				* Two points: (row, startCol) and (row, endCol)
				* We will use lowerleft of point1 and upperright of point2
				* to create smallest amount of shadow (maximum would be upperleft and lowerright)
				* 
				* (Use real coordinates and then convert to SDL coordinates)
				* Pair 1 - upperleft: (player.x+16, -(player.y+16)), (map[row][startCol].worldCol, -(map[row][startCol].worldRow)
				* Pair 2 - lowerright: (player.x+16, -(player.y+16)), (map[row][endCol].worldCol+32, -(map[row][endCol].worldRow+32))
				* Assuming line function y=a1x+b1 and y=a2x+b2
				*/

				// Pair 1, player middle point -> lowerleft
				// std::cout << "Start Col: " << startCol << " End Col: " << endCol << std::endl;
				float a1 = (float)(playerRow + 0.5 - (currentMap->getMap()[row][startCol].worldRow / TILE_HEIGHT)) / (float)(currentMap->getMap()[row][startCol].worldCol / TILE_WIDTH - (playerCol + 0.5));
				float b1 = -(float)(playerRow + 0.5) - a1 * (float)(playerCol + 0.5);
				// Pair 2, player middle point -> upperright
				float a2 = (float)(playerRow + 0.5 - (currentMap->getMap()[row][endCol].worldRow / TILE_HEIGHT + 1)) / (float)(currentMap->getMap()[row][endCol].worldCol / TILE_WIDTH + 1 - (playerCol + 0.5));
				float b2 = -(float)(playerRow + 0.5) - a2 * (float)(playerCol + 0.5);

				// For each row above, check intersection
				// y=ax+b and y=row, intersection is ((row-b)/a, row)
				for (int rowAbove = row - 1; rowAbove >= minRow; rowAbove--) {
					// Negate row for real cartesian
					// intersection 1 is on left side of intersection 2
					float intersectionCol1 = abs((-rowAbove - b1) / a1);
					float intersectionCol2 = abs((-rowAbove - b2) / a2);
					// if intersectionCol range out of range, break from loop
					if (intersectionCol2 < minCol || intersectionCol1 >= maxCol) {
						break;
					}
					// else mark tiles of the intersection range as shadowed
					// we already know the row so need to know the column
					else {
						int col1 = std::max((int)floor(intersectionCol1), playerCol);
						int col2 = std::min((int)floor(intersectionCol2), maxCol - 1);
						// std::cout << "Left Col: " << col1 << " Right Col: " << col2 << std::endl;
						for (int i = col1; i <= col2; i++) {
							currentMap->getMap()[rowAbove][i].shadowed = true;
							shadowed += 1;
						}
					}
				}
			}
		}
	}

	/* Octant 3 (includes playerRow)
	* Loop explanation:
	* - Vertical scan instead of horizontal scan
	* - Should include 45 degree ones
	*/
	std::cout << "Here" << std::endl;

	for (int col = playerCol + 1; col < maxCol; col++) {
		for (int row = playerRow + 1; row > std::max(minRow, playerRow - (col - playerCol)); row--) {
			if (currentMap->getMap()[row][col].blocker == true && currentMap->getMap()[row][col].shadowed == false) {
				// TODO: Loop to find last tile of a continous block of blocker tiles
				// TODO: Then we only cast two rays for the whole block
				// TODO: Also use two points that create smallest amount of shadow 
				int startRow = row;
				// Note that row eventually points to a non-blocker,
				// so the row++ in the for loop skips [row][col] as a bonus
				while (currentMap->getMap()[row][col].blocker == true) {
					row--;
					if (row == std::max(minRow, playerRow - (col - playerCol))) {
						break;
					}
				}
				int endRow = row - 1;


				/*
				* (Use real coordinates and then convert to SDL coordinates)
				* Pair 1 - upperleft: (player.x+16, -(player.y+16)), (map[row][startCol].worldCol, -map[row][startCol].worldRow)
				* Pair 2 - lowerright: (player.x+16, -(player.y+16)), (map[row][endCol].worldCol+32, -(map[row][endCol].worldRow+32))
				* Assuming line function y=a1x+b1 and y=a2x+b2
				*/
				// Pair 1, player middle point -> lowerleft
				float a1 = (float)(playerRow + 0.5 - (currentMap->getMap()[startRow][col].worldRow / TILE_HEIGHT)) / (float)(currentMap->getMap()[startRow][col].worldCol / TILE_WIDTH - (playerCol + 0.5));
				float b1 = -(float)(playerRow + 0.5) - a1 * (float)(playerCol + 0.5);
				// Pair 2, player middle point -> upperright
				float a2 = (float)(playerRow + 0.5 - (currentMap->getMap()[endRow][col].worldRow / TILE_HEIGHT + 1)) / (float)(currentMap->getMap()[endRow][col].worldCol / TILE_WIDTH + 1 - (playerCol + 0.5));
				float b2 = -(float)(playerRow + 0.5) - a2 * (float)(playerCol + 0.5);
				// For each col to the right, check intersection
				// y=ax+b and x=col, intersection is (col, a*col+b)
				for (int colRight = col + 1; colRight < maxCol; colRight++) {
					// Negate row for real cartesian
					// intersection 1 is on lower side of intersection 2
					float intersectionRow1 = abs(a1 * colRight + b1);
					float intersectionRow2 = abs(a2 * colRight + b2);
					// Row1 is above of Row2, i.e. Row1 < Row2 in SDL coordinate (vice versa in real)
					// if intersectionRow range out of range, break from loop
					if (intersectionRow2 < minRow || intersectionRow1 >= maxRow) {
						break;
					}
					// else mark tiles of the intersection range as shadowed
					// we already know the col so need to know the row
					else {
						int row1 = (int)ceil(intersectionRow1);
						int row2 = (int)floor(intersectionRow2);
						//int row1 = std::min((int)ceil(intersectionRow1), playerRow);
						//int row2 = std::max((int)floor(intersectionRow2), minRow);
						// std::cout << "From row " << row2 << " to " << row1 << std::endl;
						// Vertical scan would require the lowest cell to be ignored
						// i.e. i=row2 should be excluded
						for (int i = row2; i <= row1; i++) {
							currentMap->getMap()[i][colRight].shadowed = true;
							shadowed += 1;
						}
					}
				}
			}
		}
	}
	// std::cout << "Total shadowed: " << shadowed << std::endl;
}

void cGameScene::shadowCastingExperimentalFOV() {
	/*
	* FOV: experimental quadrant shadow casting
	* Divide the screen into four parts: north, south, east and west
	* Each part does NOT contain the row/col player occupies
	* But with all four parts combined, all tiles (except for player tile) are checked
	* 
	* For north and south directions, scan horizontally
	* For west and east directions, scan vertically
	*/

	// Remember i is for X which is horizontal and j is for Y which is vertical
	// TODO: We also completely ignore a part if player is too close to it
	// TODO: For example, too close to bottom then we only check upper part

	int minCol = std::max((screen.topLeftViewport.x - screen.biasCol) / TILE_WIDTH, 0);
	int minRow = std::max((screen.topLeftViewport.y - screen.biasRow) / TILE_HEIGHT, 0);
	int maxCol = std::min(minCol + screen.topLeftViewport.w / TILE_WIDTH, currentMap->getMapWidth()-1);
	int maxRow = std::min(minRow + screen.topLeftViewport.h / TILE_HEIGHT, currentMap->getMapHeight()-1);

	int playerCol = playerEntity.worldX / TILE_WIDTH;	// Pixel to index
	int playerRow = playerEntity.worldY / TILE_HEIGHT;

	// Clean all shadows
	for (int col = minCol; col <= maxCol; col++) {
		for (int row = minRow; row <= maxRow; row++) {
			currentMap->getMap()[row][col].shadowed = false;
		}
	}

	int shadowed = 0;

	/* Direction North
	* 
	* for each row <= playerRow && row > minRow:
	*	find consecutive block of horizontal sight-blocking tiles;
	*   we have a starting tile (startTile) on left and an ending tile (endTile) on right
	*	note that we reference all tiles by their upperleft col/row
	*	if startTile.col <= playerCol:
	*		use startTile lowerleft point as p1;
	*	else:
	*		use startTile upperleft point as p1;
	*	if endTile.col < playerCol:
	*		use endTile upperright point as p2;
	*	else:
	*		use endTile lowerright point as p2;
	*	// For above algorithm check FOV_Scan_00.jpg for demonstration
	*	let playerTile middle point be p0;
	*	find two lines, line1: p0->p1 and line2: p0->p2;
	*	
	*	for each scanRow <= row && scanRow > minRow:
	*		// We need to use intersection of PREVIOUS ROW for the current row,
	*		// (Previous row means the row south of current row)
	*		// because we are scanning from south towards north.
	*		// This is why we start from row instead of row-1
	*		// This is also why scanRow does NOT need to reach minRow
	*		// Because the code is going to reach minRow by subtracting 1
	*		find intersection of line1 and scanRow, s1;
	*		find intersection of line2 and scanRow, s2;
	*		// s1 must be on left side of s2
	*		// s1 and s2 are for the row of scanRow-1 (we use UPPERLEFT point to reference a tile)
	*		let's say s1 is (scanRow, col1), s2 is (scanRow, col2)
	*		let col1 be the next integer (e.g. if 11.2, use 12);
	*		let col2 be the previous integer minus 1 (e.g. if 20.6, use 19, not 20);
	*		shadow map[scanRow-1, col1]~map[scanRow-1, col2], both endpoints included;
	*		
	*		// e.g. s1 (4, 3.2), s2 (4, 10.9)
	*		// we should shadow (3, 4), (3, 5), (3, 6), (3, 7), (3, 8), (3, 9)
	*		// All tiles shadows are FULLY SHADOWED so we can ignore them in other scans
	*/
	for (int row = playerRow - 1; row > minRow; row--) {
		for (int col = minCol; col < maxCol; col++) {
			if (currentMap->getMap()[row][col].blocker == true) {
				if (DEBUG) { std::cout << row << ", " << col << std::endl; }
				int startCol = col;
				// Note that col eventually points to a non-blocker,
				// so the col++ in the for loop skips [row][col] as a bonus
				while (currentMap->getMap()[row][col].blocker == true) {
					col++;
					if (col == maxCol) {
						break;
					}
				}
				int endCol = col - 1;

				SDL_Point p1 = SDL_Point{ startCol, row };
				SDL_Point p2 = SDL_Point{ endCol, row };
				
				// Check comments on top for explanation
				if (startCol <= playerCol) {
					// Use lower left corner
					p1.y += 1;
				}
				else {
					// Use upper left corner (the default reference point of a tile)
					// Do nothing
				}

				if (endCol < playerCol) {
					// Use upper right corner
					p2.x += 1;
				}
				else {
					// Use lower right corner
					p2.x += 1;
					p2.y += 1;
				}

				// Player tile middle point
				float playerMidCol = (float)playerCol + (float)0.5;
				float playerMidRow = (float)playerRow + (float)0.5;

				if (DEBUG) { std::cout << "(" << playerMidRow << ", " << playerMidCol << ") to " << "(" << p1.y << ", " << p1.x << ") and (" << p2.y << ", " << p2.x << ")\n"; }

				// Pair 1, p0->p1
				// Remember to convert to real coordinates (invert row): y=ax+b
				float a1 = (float)((p1.y * -1) - (playerMidRow * -1)) / (float)(p1.x - playerMidCol);
				float b1 = (float)(playerMidRow * -1) - a1 * (float)(playerMidCol);
	
				// Pair 2, p0->p2
				float a2 = (float)((p2.y * -1) - (playerMidRow * -1)) / (float)(p2.x - playerMidCol);
				float b2 = (float)(playerMidRow * -1) - a2 * (float)(playerMidCol);

				// For each scanRow starting from row, check intersection
				// For y=ax+b and y=row, intersection is ((row-b)/a, row)
				// why not starting from row-1 and ending at minRow? 
				// Check explanation in notes
				for (int rowScan = row; rowScan >= minRow + 1; rowScan--) {
					// std::cout << rowScan << " ," << minRow + 1 << std::endl;
					// Negate row for real cartesian
					// s1 is on left side of s2
					float intersectionCol1 = (-rowScan - b1) / a1;
					float intersectionCol2 = (-rowScan - b2) / a2;
					// if intersectionCol range out of range, break from loop
					if (intersectionCol2 < minCol || intersectionCol1 >= maxCol) {
						break;
					}
					// Adjust col to integers and remove head and tail tiles
					int colLeft = std::max((int)ceil(intersectionCol1), minCol);
					int colRight = std::min((int)floor(intersectionCol2) - 1, maxCol);
					// To reduce artifact from shadows projected by single tiles
					if (startCol == endCol) {
						// if tile west of player, reduce 1 further from colLeft
						if (endCol < playerCol) {
							colLeft += 1;
						}
						// if tile east of player, reduce 1 further from colRight
						if (endCol > playerCol) {
							// I don't know why but I need to squeeze colLeft too...
							colLeft += 1;
							colRight -= 1;
						}
						// ignore if startRow = endRow == playerRow
					}
					// else mark tiles of the intersection range as shadowed
					// but we need to subtract 32 from row as s1/s2 are for row above
					// that's why we can stop the scan at minRow + 1
					// because s1 and s2 or minRow + 1 are actually for minRow
					if (DEBUG) { std::cout << "RowScan: " << rowScan << " Left Col: " << colLeft << " Right Col: " << colRight << std::endl; }
					for (int col = colLeft; col <= colRight; col++) {
						currentMap->getMap()[rowScan - 1][col].shadowed = true;
						shadowed += 1;
					}
				}
			}
		}
	}

	/* Direction South
	*
	* for each row >= playerRow+1 && row < maxRow:
	*	find consecutive block of horizontal sight-blocking tiles;
	*   we have a starting tile (startTile) on left and an ending tile (endTile) on right
	*	note that we reference all tiles by their upperleft col/row
	*	if startTile.col <= playerCol:
	*		use startTile upperleft point as p1;
	*	else:
	*		use startTile lowerleft point as p1;
	*	if endTile.col < playerCol:
	*		use endTile lowerright point as p2;
	*	else:
	*		use endTile upperright point as p2;
	*	let playerTile middle point be p0;
	*	find two lines, line1: p0->p1 and line2: p0->p2;
	*
	*	for each scanRow >= row+1 && scanRow <= maxRow:
	*		// We need to use intersection of current row,
	*		// because we are scanning from north towards south.
	*		find intersection of line1 and scanRow, s1;
	*		find intersection of line2 and scanRow, s2;
	*		// s1 must be on left side of s2
	*		// s1 and s2 are for the row of scanRow-1 (we use UPPERLEFT point to reference a tile)
	*		let's say s1 is (scanRow, col1), s2 is (scanRow, col2)
	*		let col1 be the next integer (e.g. if 11.2, use 12);
	*		let col2 be the previous integer minus 1 (e.g. if 20.6, use 19, not 20);
	*		shadow map[scanRow-1, col1]~map[scanRow-1, col2], both endpoints included;
	*
	*		// e.g. s1 (4, 3.2), s2 (4, 10.9)
	*		// we should shadow (3, 4), (3, 5), (3, 6), (3, 7), (3, 8), (3, 9)
	*		// All tiles shadows are FULLY SHADOWED so we can ignore them in other scans
	*/
	for (int row = playerRow + 1; row < maxRow; row++) {
		for (int col = minCol+1; col < maxCol; col++) {
			if (currentMap->getMap()[row][col].blocker == true) {
				int startCol = col;
				// Note that col eventually points to a non-blocker,
				// so the col++ in the for loop skips [row][col] as a bonus
				while (currentMap->getMap()[row][col].blocker == true) {
					col++;
					if (col == maxCol) {
						break;
					}
				}
				int endCol = col - 1;

				SDL_Point p1 = SDL_Point{ startCol, row };
				SDL_Point p2 = SDL_Point{ endCol, row };

				// Check comments on top for explanation
				if (startCol <= playerCol) {
					// Use upper left corner
				}
				else {
					// Use lower left corner
					p1.y += 1;
				}

				if (endCol < playerCol) {
					// Use lower right corner
					p2.x += 1;
					p2.y += 1;
				}
				else {
					// Use upper right corner
					p2.x += 1;
				}

				// Player tile middle point
				float playerMidCol = (float)playerCol + (float)0.5;
				float playerMidRow = (float)playerRow + (float)0.5;


				// Pair 1, p0->p1
				// Remember to convert to real coordinates (invert row): y=ax+b
				float a1 = (float)((p1.y * -1) - (playerMidRow * -1)) / (float)(p1.x - playerMidCol);
				float b1 = (float)(playerMidRow * -1) - a1 * (float)(playerMidCol);

				// Pair 2, p0->p2
				float a2 = (float)((p2.y * -1) - (playerMidRow * -1)) / (float)(p2.x - playerMidCol);
				float b2 = (float)(playerMidRow * -1) - a2 * (float)(playerMidCol);

				// For each scanRow starting from row + 1, check intersection
				// For y=ax+b and y=row, intersection is ((row-b)/a, row)
				for (int rowScan = row + 1; rowScan <= maxRow; rowScan++) {
					// std::cout << rowScan << " ," << minRow + 1 << std::endl;
					// Negate row for real cartesian
					// s1 is on left side of s2
					float intersectionCol1 = (-rowScan - b1) / a1;
					float intersectionCol2 = (-rowScan - b2) / a2;
					
					// if intersectionCol range out of range, break from loop
					if (intersectionCol2 < minCol || intersectionCol1 >= maxCol) {
						break;
					}
					// Adjust col to integers and remove head and tail tiles
					int colLeft = std::max((int)ceil(intersectionCol1), minCol);
					int colRight = std::min((int)floor(intersectionCol2) - 1, maxCol);
					if (DEBUG) { std::cout << intersectionCol1 << "->" << colLeft << ", " << intersectionCol2 << "->" << colRight << std::endl; }
					// To reduce artifact from shadows projected by single tiles
					if (startCol == endCol) {
						// if tile west of player, reduce 1 further from colLeft
						if (endCol < playerCol) {
							colLeft += 1;
							colRight -= 1;
						}
						// if tile east of player, reduce 1 further from colRight
						if (endCol > playerCol) {
							// I don't know why but I need to squeeze colLeft too...
							colLeft += 1;
							colRight -= 1;
						}
						// ignore if startRow = endRow == playerRow
					}
					// else mark tiles of the intersection range as shadowed
					// but we need to subtract 32 from row as s1/s2 are for row above
					// that's why we can stop the scan at minRow + 1
					// because s1 and s2 or minRow + 1 are actually for minRow
					for (int col = colLeft; col <= colRight; col++) {
						currentMap->getMap()[rowScan][col].shadowed = true;
						shadowed += 1;
					}
				}
			}
		}
	}

	/* Direction East
	*
	* for each col > playerCol && col < maxCol:
	*	find consecutive block of vertical sight-blocking tiles;
	*   we have a starting tile (startTile) on top and an ending tile (endTile) on bottom
	*	note that we reference all tiles by their upperleft col/row
	*	if startTile.row <= playerRow:
	*		use startTile upperleft point as p1;
	*	else:
	*		use startTile upperright point as p1;
	*	if endTile.row < playerRow:
	*		use endTile lowerright point as p2;
	*	else:
	*		use endTile lowerleft point as p2;
	*	// For above algorithm check FOV_Scan_00.jpg for demonstration
	*	let playerTile middle point be p0;
	*	find two lines, line1: p0->p1 and line2: p0->p2;
	*
	*	for each scanCol < maxCol && scanCol >= col:
	*		// We need to use intersection of PREVIOUS COL for the current col,
	*		// (Previous col means the col on the left side of current col)
	*		// because we are scanning from west towards east.
	*		// This is why we start from col instead of col+1
	*		// This is also why scanCol does NOT need to reach maxCol
	*		// Because the code is going to reach maxCol by adding 1
	*		find intersection of line1 and scanRow, s1;
	*		find intersection of line2 and scanRow, s2;
	*		// s1 must be on north side of s2
	*		// s1 and s2 are for the col of scanCol+1 (we use UPPERLEFT point to reference a tile)
	*		let's say s1 is (row1, scanCol), s2 is (row2, scanCol), row1<row2 (in SDL coordinate)
	*		let row1 be the next integer (e.g. if 11.2, use 12);
	*		let row2 be the previous integer minus 1 (e.g. if 20.6, use 19, not 20);
	*		shadow map[row1, scanCol+1]~map[row2, scanCol+1], both endpoints included;
	*
	*		// e.g. s1 (3.2, 4), s2 (10.9, 4)
	*		// we should shadow (3, 4), (3, 5), (3, 6), (3, 7), (3, 8), (3, 9)
	*		// All tiles shadows are FULLY SHADOWED so we can ignore them in other scans
	*/

	for (int col = playerCol + 1; col < maxCol; col++) {
		for (int row = minRow; row < maxRow; row++) {
			if (currentMap->getMap()[row][col].blocker == true) {
				int startRow = row;
				// Note that row eventually points to a non-blocker,
				// so the row++ in the for loop skips [row][col] as a bonus
				while (currentMap->getMap()[row][col].blocker == true) {
					row++;
					if (row == maxRow) {
						break;
					}
				}
				int endRow = row - 1;

				SDL_Point p1 = SDL_Point{ col, startRow };
				SDL_Point p2 = SDL_Point{ col, endRow };

				// Check comments on top for explanation
				if (startRow <= playerRow) {
					// Use upper left corner (the default reference point of a tile)
					// Do nothing
				}
				else {
					// Use upper right corner
					p1.x += 1;
				}

				if (endRow < playerRow) {
					// Use lower right corner
					p2.x += 1;
					p2.y += 1;
				}
				else {
					// Use lower left corner
					p2.y += 1;
				}

				// Player tile middle point
				float playerMidCol = (float)playerCol + (float)0.5;
				float playerMidRow = (float)playerRow + (float)0.5;

				// Pair 1, p0->p1
				// Remember to convert to real coordinates (invert row): y=ax+b
				float a1 = (float)((p1.y * -1) - (playerMidRow * -1)) / (float)(p1.x - playerMidCol);
				float b1 = (float)(playerMidRow * -1) - a1 * (float)(playerMidCol);

				// Pair 2, p0->p2
				float a2 = (float)((p2.y * -1) - (playerMidRow * -1)) / (float)(p2.x - playerMidCol);
				float b2 = (float)(playerMidRow * -1) - a2 * (float)(playerMidCol);

				// For each colScan starting from col+1, check intersection
				// For y=ax+b and x=colScan, intersection is (a*colScan+b, colScan)
				// We sohuld start from col+1 and ending at maxCol (This is different from North)
				// Because the tiles are referenced at upperleft point
				for (int colScan = col+1; colScan <= maxCol; colScan++) {
					// Negate row for real cartesian, so need to revert that
					// s1 is on north side of s2
					float intersectionRow1 = abs(a1 * colScan + b1);
					float intersectionRow2 = abs(a2 * colScan + b2);
					// std::cout << "Row: " << row-screen.biasRow << " Cols: " << intersectionRow1 << " to " << intersectionRow2 << std::endl;
					// if intersectionRow range out of screen, break from loop
					if (intersectionRow2 < minRow || intersectionRow1 >= maxRow) {
						break;
					}
					// Adjust col to integers and remove head and tail partial tiles
					int rowUp = std::max((int)ceil(intersectionRow1), minRow);
					int rowDown = std::min((int)floor(intersectionRow2) - 1, maxRow);
					// To reduce artifact from shadows projected by single tiles
					if (startRow == endRow) {
						// if tile north of player, reduce 1 further from rowDown
						if (endRow < playerRow) {
							rowDown -= 1;
						}
						// if tile south of player, reduce 1 further from rowUp
						if (startRow > playerRow) {
							rowUp += 1;
						}
						// ignore if startRow = endRow == playerRow
					}
					if (DEBUG) { std::cout << intersectionRow1 << "->" << rowUp << ", " << intersectionRow2 << "->" << rowDown << std::endl; }
					// else mark tiles of the intersection range as shadowed
					// but we need to subtract 32 from row as s1/s2 are for row above
					// that's why we can stop the scan at minRow + 1
					// because s1 and s2 or minRow + 1 are actually for minRow
					for (int row = rowUp; row <= rowDown; row++) {
						currentMap->getMap()[row][colScan].shadowed = true;
						shadowed += 1;
					}
				}
			}
		}
	}

	/* Direction West
	*
	* for each col < playerCol && col >= maxCol:
	*	find consecutive block of vertical north-south sight-blocking tiles;
	*   we have a starting tile (startTile) on top and an ending tile (endTile) on bottom
	*	note that we reference all tiles by their upperleft col/row
	*	if startTile.row <= playerRow:
	*		use startTile upperleft point as p1;
	*	else:
	*		use startTile upperright point as p1;
	*	if endTile.row < playerRow:
	*		use endTile lowerleft point as p2;
	*	else:
	*		use endTile lowerright point as p2;
	*	// For above algorithm check FOV_Scan_00.jpg for demonstration
	*	let playerTile middle point be p0;
	*	find two lines, line1: p0->p1 and line2: p0->p2;
	*
	*	for each scanCol > minCol && scanCol <= col:
	*		// We need to use intersection of PREVIOUS COL for the current col,
	*		// (Previous col means the col on the right side of current col)
	*		// because we are scanning from east towards west.
	*		// This is why we start from col instead of col-1
	*		// This is also why scanCol does NOT need to reach minCol
	*		// Because the code is going to reach minCol by subtracting 1
	*		find intersection of line1 and scanCol, s1;
	*		find intersection of line2 and scanCol, s2;
	*		// s1 must be on north side of s2
	*		// s1 and s2 are for the col of scanCol+1 (we use UPPERLEFT point to reference a tile)
	*		let's say s1 is (row1, scanCol), s2 is (row2, scanCol), row1<row2 (in SDL coordinate)
	*		let row1 be the next integer (e.g. if 11.2, use 12);
	*		let row2 be the previous integer minus 1 (e.g. if 20.6, use 19, not 20);
	*		shadow map[row1, scanCol+1]~map[row2, scanCol+1], both endpoints included;
	*
	*		// e.g. s1 (3.2, 4), s2 (10.9, 4)
	*		// we should shadow (3, 4), (3, 5), (3, 6), (3, 7), (3, 8), (3, 9)
	*		// All tiles shadows are FULLY SHADOWED so we can ignore them in other scans
	*/

	for (int col = playerCol - 1; col > minCol; col--) {
		for (int row = minRow+1; row < maxRow; row++) {
			if (currentMap->getMap()[row][col].blocker == true) {
				int startRow = row;
				// Note that row eventually points to a non-blocker,
				// so the row++ in the for loop skips [row][col] as a bonus
				while (currentMap->getMap()[row][col].blocker == true) {
					row++;
					if (row == maxRow) {
						break;
					}
				}
				int endRow = row - 1;

				SDL_Point p1 = SDL_Point{ col, startRow };
				SDL_Point p2 = SDL_Point{ col, endRow };

				// Check comments on top for explanation
				if (startRow < playerRow) {
					// Use upper left corner (the default reference point of a tile)
					// Do nothing
				}
				else {
					// Use upper right corner
					p1.x += 1;
				}

				if (endRow < playerRow) {
					// Use lower left corner					
					p2.y += 1;
				}
				else {
					// Use lower right corner
					p2.x += 1;
					p2.y += 1;
				}

				// Player tile middle point
				float playerMidCol = (float)playerCol + (float)0.5;
				float playerMidRow = (float)playerRow + (float)0.5;

				// Pair 1, p0->p1
				// Remember to convert to real coordinates (invert row): y=ax+b
				float a1 = (float)((p1.y * -1) - (playerMidRow * -1)) / (float)(p1.x - playerMidCol);
				float b1 = (float)(playerMidRow * -1) - a1 * (float)(playerMidCol);

				// Pair 2, p0->p2
				float a2 = (float)((p2.y * -1) - (playerMidRow * -1)) / (float)(p2.x - playerMidCol);
				float b2 = (float)(playerMidRow * -1) - a2 * (float)(playerMidCol);

				// For each colScan starting from col, check intersection
				// For y=ax+b and x=colScan, intersection is (a*colScan+b, colScan)
				// We sohuld start from col and ending at minCol+1 (This is different from East)
				// Because the tiles are referenced at upperleft point
				for (int colScan = col; colScan > minCol; colScan--) {
					// Negate row for real cartesian, so need to revert that
					// s1 is on north side of s2
					float intersectionRow1 = abs(a1 * colScan + b1);
					float intersectionRow2 = abs(a2 * colScan + b2);
					// if intersectionRow range out of screen, break from loop
					if (intersectionRow1 > maxRow || intersectionRow2 < minRow) {
						break;
					}
					// Adjust col to integers and remove head and tail partial tiles
					int rowUp = std::max((int)ceil(intersectionRow1), minRow);
					int rowDown = std::min((int)floor(intersectionRow2) - 1, maxRow);
					// To reduce artifact from shadows projected by single tiles
					if (startRow == endRow) {
						// if tile north of player, reduce 1 further from rowDown
						if (endRow < playerRow) {
							rowDown -= 1;
						}
						// if tile south of player, reduce 1 further from rowUp
						if (startRow > playerRow) {
							rowUp += 1;
						}
						// ignore if startRow = endRow == playerRow
					}
					if (DEBUG) { std::cout << intersectionRow1 << "->" << rowUp << ", " << intersectionRow2 << "->" << rowDown << std::endl; }
					// else mark tiles of the intersection range as shadowed
					// but we need to subtract 32 from row as s1/s2 are for row above
					// that's why we can stop the scan at minRow + 1
					// because s1 and s2 or minRow + 1 are actually for minRow
					for (int row = rowUp; row <= rowDown; row++) {
						currentMap->getMap()[row][colScan-1].shadowed = true;
						shadowed += 1;
					}
				}
			}
		}
	}
	// std::cout << "Total shadowed: " << shadowed << std::endl;
}


// Map scrolling
bool cGameScene::canScroll(int dir) {
	// Leaves gaps between player and border for better visual
	switch (dir) {
	case EAST: {
		if (playerEntity.worldX <= MIN_WIDTH_SCROLL) { return false; }
		else { return ((screen.topLeftViewport.x - screen.biasCol) / TILE_WIDTH + screen.topLeftViewport.w / TILE_WIDTH < currentMap->getMapWidth()); }
	}
	case WEST: {
		if (playerEntity.worldX >= currentMap->getMapWidth()*TILE_WIDTH - MIN_WIDTH_SCROLL) { return false; }
		else { return ((screen.topLeftViewport.x - screen.biasCol) / TILE_WIDTH > 0); }
	}
	case SOUTH: {
		if (playerEntity.worldY <= MIN_HEIGHT_SCROLL) { return false; }
		return ((screen.topLeftViewport.y - screen.biasRow) / TILE_HEIGHT + screen.topLeftViewport.h / TILE_HEIGHT < currentMap->getMapHeight());
	}
	case NORTH: {
		if (playerEntity.worldY >= currentMap->getMapHeight()*TILE_HEIGHT - MIN_HEIGHT_SCROLL) { return false; }
		return ((screen.topLeftViewport.y - screen.biasRow) / TILE_HEIGHT > 0);
	}
	}
}

void cGameScene::scroll(int dir) {
	if (canScroll(dir)) {
		screen.Update(dir);
		this->shadowCastingExperimentalFOV();
	}
	else {
		// Do nothing
		std::cout << "Cannot Scroll" << std::endl;
	}
}

void cGameScene::createItemOnEntity(cEntity& e, std::string id, cItemFactory& itemFactory) {
	e.addItem(itemFactory.createItem(id));
}

void cGameScene::changeMap() {
	if (currentMap->getMapChange().size() == 0) {
		return;
	}
	for (auto c : currentMap->getMapChange()) {
		if (playerEntity.worldX == c.sourceLoc.x && playerEntity.worldY == c.sourceLoc.y) {
			currentMap = (*map)[c.destMapID];
			playerEntity.worldX = c.destLoc.x;
			playerEntity.worldY = c.destLoc.y;
			// Re-init screen for proper alignment
			// TODO: This does NOT solve the issue of black strips moving back
			// TODO: from MERCHANT1 to ULTIMA, two options:
			// TODO: either we can memorize bias for each map change point when player first makes the jump
			// TODO: or (this is probably better but more complicated) modify Init()
			screen.Init(playerEntity, currentMap->getMapWidth(), currentMap->getMapHeight());
			break;
		}
	}
}

void cGameScene::run() {
	// FPS timers
	Uint32 startTime = 0;
	Uint32 expiredTicks = 0;
	cTimer sessionTimer;
	cTimer cycleTimer;
	SDL_Event e;
	sessionTimer.start();

	bool quit = false;

	while (!quit) {
		expiredTicks = 0;
		cycleTimer.start();
		playerEntity.updateFrame();
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					if (inConversationState) {
						// Enter conversation mode
					}
					else if (ai.canMoveDirection(&playerEntity, NORTH, currentMap)) {
						updatePlayer(NORTH);
						scroll(NORTH);
					}
					break;
				case SDLK_DOWN:
					if (inConversationState) {
						// Enter conversation mode
					}
					else if (ai.canMoveDirection(&playerEntity, SOUTH, currentMap)) {
						updatePlayer(SOUTH);
						scroll(SOUTH);
					}
					break;
				case SDLK_LEFT:
					if (inConversationState) {
						// Enter conversation mode
					}
					else if (ai.canMoveDirection(&playerEntity, WEST, currentMap)) {
						updatePlayer(WEST);
						scroll(WEST);
					}
					break;
				case SDLK_RIGHT:
					if (inConversationState) {
						// Enter conversation mode
					}
					else if (ai.canMoveDirection(&playerEntity, EAST, currentMap)) {
						updatePlayer(EAST);
						scroll(EAST);
					}
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				case SDLK_e:
					// trigger map change
					changeMap();
					break;
				case SDLK_t:
					// talk, should set keystate to ENTER_TALK
					// So expecting an arrow key afterwards
					inConversationState = true;
				default:
					break;
				}
				// Each key down triggers npc movement
				for (auto n : currentMap->getNPC()) {
					ai.moveRandomly(n, currentMap);
				}
			}
		}
		renderer->ClearScreen();
		draw();
		renderer->RenderText(0, 0, "Version: 0.55\n Date: 2023-02-20", "test");
		renderer->Show();

		Uint32 expectedTimeSpent = MILLISECONDS_PER_FRAME;
		Uint32 realizedTimeSpent = cycleTimer.getTicks();
		if (expectedTimeSpent > realizedTimeSpent)
		{
			// Wait remaining time
			// std::cout << "Expected: " << expectedTimeSpent << " - Realized: " << realizedTimeSpent << std::endl;
			SDL_Delay(expectedTimeSpent - realizedTimeSpent);
		}
	}
}