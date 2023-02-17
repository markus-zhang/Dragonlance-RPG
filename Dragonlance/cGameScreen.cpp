#include "cGameScreen.h"
#include "Parameters.h"
#include "SDL.h"
#include "cPlayer.h"
#include <iostream>

void cGameScreen::Init(cPlayer player, int mapWidth=0, int mapHeight=0)
{
	/*
	* We should always try to center on the player, except when it is not possible
	* Let's say the screen is 800*640, which is 25*20 for 32*32 tile/sprite
	* Assuming player is at world coordinate (20, 5)
	* So we want to draw horizontal tiles in [8, 33)
	* For vertical tiles, however, we cannot get into negative values
	* So it's going to [0, 20)
	* 
	* The above assumes that map always cover the on-screen rectangle
	* For smaller maps, e.g. if the map is of 25*25
	* Horizontally we won't be able to draw [8, 33), so instead we draw [0, 25)
	* 
	* If the map can be fit in one screen, we simply draw the full map on screen
	* But will try to centralize it
	*/
	this->mapWidth = mapWidth;
	this->mapHeight = mapHeight;

	int screenCenterX = ((topLeftViewport.x + topLeftViewport.w / 2)/32)*32;
	int screenCenterY = ((topLeftViewport.y + topLeftViewport.h / 2)/32)*32;

	biasCol = ((screenCenterX - player.worldX)/TILE_WIDTH)*TILE_WIDTH;
	biasRow = ((screenCenterY - player.worldY)/TILE_HEIGHT)*TILE_HEIGHT;

	/*
	* Check the distance between playerX and left/right border (0 and 1258 in ULTIMA)
	* Check the distance between playerY and up/down border (0 and 1258 in ULTIMA)
	* Say playerX=160 and ScreenWidth=960, biasX should be -160
	* Say playerY=1130 and ScreenHeight=640, biasY should be -128
	*/
	if (player.worldX < GAME_SCREEN_WIDTH / 2) {
		// Too close to left map border, set bias to 0
		// so that the left map border matches the left screen border
		biasCol = 0;
	}
	else if (mapWidth - TILE_WIDTH - player.worldX < GAME_SCREEN_WIDTH / 2) {
		// To close to right map border, set bias to
		// so that the right map border matches the right screen border
		biasCol = -(mapWidth - GAME_SCREEN_WIDTH);
	}

	if (player.worldY < GAME_SCREEN_HEIGHT / 2) {
		// Too close to left map border, set bias to 0
		// so that the left map border matches the left screen border
		biasCol = 0;
	}
	else if (mapHeight - TILE_HEIGHT - player.worldY < GAME_SCREEN_HEIGHT / 2) {
		// To close to right map border, set bias to
		// so that the right map border matches the right screen border
		biasCol = -(mapHeight - GAME_SCREEN_HEIGHT);
	}

	std::cout << "Screen: " << screenCenterX << " " << screenCenterY << " " << biasCol << " " << biasRow << std::endl;
	std::cout << "Player: " << player.worldX << " " << player.worldY << std::endl;
}

void cGameScreen::Update(int dir) {
	switch (dir) {
	case EAST: {
		// Note that biases are extracted, so should be smaller when moving east
		biasCol -= TILE_WIDTH;
		break;
	}
	case SOUTH: {
		biasRow -= TILE_WIDTH;
		break;
	}
	case WEST: {
		biasCol += TILE_WIDTH;
		break;
	}
	case NORTH: {
		biasRow += TILE_WIDTH;
		break;
	}
	default: {
		std::cout << "Direction must be between " << EAST << " and " << NORTH << std::endl;
		break;
	}
	}
}