#pragma once

/*
* Rules of moving a game screen:
*	- We don't need to move screen with every move of the player
*	- let's define a distance from 
*/

#include "cScreen.h"

class cPlayer;

class cGameScreen : public cScreen {
public:
	cGameScreen() = default;
	cGameScreen(int w, int h, int ulx, int uly) : 
		cScreen(w, h, ulx, uly), biasCol(0), biasRow(0), mapWidth(0), mapHeight(0) {}
	void Init(cPlayer player, int mapWidth, int mapHeight);
	void Update(int dir);
public:
	/*
	* When player moves, the screen moves with him
	* The biases are calculated for rendering the tiles at correct positions
	*/
	int biasCol;
	int biasRow;
	int mapWidth;
	int mapHeight;
};