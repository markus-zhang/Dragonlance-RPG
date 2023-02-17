#include "cMap.h"
#include "cGraphics.h"
#include "cGameScreen.h"
#include "Parameters.h"
#include "SDL.h"
#include <iostream>

cMap::cMap(std::string spriteSheetId, int mapWidth, int mapHeight, std::vector<int> tmxData) {
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;
	map.resize(mapHeight);
	this->spriteSheetId = spriteSheetId;
	int i = 0;
	int j = 0; 
	for (int k = 0; k < mapWidth * mapHeight; k++) {
		map.at(j).push_back(int2Tile(tmxData[k] - 1, i * 32, j * 32, 32, 32));
		i++;
		if (i == mapWidth) {
			j++;
			i = 0;
		}
	}
}

cTile cMap::int2Tile(int index, int worldCol, int worldRow, int width, int height) {
	/*
		1. tmx file has tile index from 1, not 0
		2. need to manually assign attributes to every tile
			for now let's hardcode everything except for
			spriteSheetId, spriteSheetIndex, worldCol and worldRow
	*/
	// cTile t(spriteSheetId, index, 0, 0, 0, worldCol, worldRow, 32, 32);
	return cTile(spriteSheetId, index, 0, 0, worldCol, worldRow, 32, 32);
}

std::vector<std::vector<cTile>>& cMap::getMap() {
	return map;
}

SDL_Rect cMap::getMapPixelDimension() {
	return SDL_Rect{0, 0, TILE_WIDTH * mapWidth, TILE_HEIGHT * mapHeight};
}

void cMap::draw(cGraphics* g, const cGameScreen& s) {
	/*
	* We need to get minX, maxX, minY and maxY for the two loops
	*/

	int minCol = std::max((s.topLeftViewport.x - s.biasCol) / TILE_WIDTH, 0);
	int minRow = std::max((s.topLeftViewport.y - s.biasRow) / TILE_HEIGHT, 0);
	int maxCol = std::min(minCol + s.topLeftViewport.w / TILE_WIDTH, mapWidth);
	int maxRow = std::min(minRow + s.topLeftViewport.h / TILE_HEIGHT, mapHeight);

	for (int col = minCol; col < maxCol; col++) {
		for (int row = minRow; row < maxRow; row++) {
			/*
			* The biases must be considered to render tiles properly when scrolling
			* Each j represents one line, so [1,2] actually matches map[2][1]
			* This is row 2, col 1. Row is y and col is x.
			*/
			if (!map[row][col].shadowed) {
				g->RenderTileWithBias(map[row][col], s.biasCol, s.biasRow);
			}
		}
	}
	return;
}