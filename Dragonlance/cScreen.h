#pragma once
#include "SDL.h"

/*
* cScreen contains boundary information about which cTiles/cEntities should be drawn.
* Each scene should have a cScreen objects.
* Each game should contain multiple scenes.
*/

/*
* The Screen should wrap a viewport SDL_Rect that can be projected to any place of the monitor
* In this way it's easier to change the size of each screen
* This also measn each scene probably have multiple cScreen objects
*/


class cScreen {
public:
	cScreen() : topLeftViewport(SDL_Rect{0,0,0,0}) {}
	cScreen(int w, int h, int ulx, int uly);
	cScreen(SDL_Rect r);
	void Resize(int w, int h);
public:
	// width and height should be adjustable by keystrokes
	SDL_Rect topLeftViewport;
};