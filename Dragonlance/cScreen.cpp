#include "cScreen.h"
#include "Parameters.h"
#include "SDL.h"
#include <iostream>

cScreen::cScreen(int w, int h, int ulx, int uly) {
	topLeftViewport.w = w;
	topLeftViewport.h = h;
	topLeftViewport.x = ulx;
	topLeftViewport.y = uly;
}

cScreen::cScreen(SDL_Rect r) {
	topLeftViewport = r;
}

void cScreen::Resize(int w, int h)
{
	topLeftViewport.w = w;
	topLeftViewport.h = h;
}