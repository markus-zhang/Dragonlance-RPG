#include "cGraphics.h"
#include "cTile.h"
#include "cMapResmanager.h"
#include "cScreen.h"
#include "cGameScreen.h"
#include "cGameScene.h"
#include "cTimer.h"
#include "Parameters.h"
#include "cPlayer.h"
#include "cMonsterOrc.h"
#include "cMap.h"
#include "tinyxml2.h"
#include "SDL_FontCache.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int main(int argc, char* argv[]) {
	cGraphics* graphics = new cGraphics(1024, 800, "Dragonlance");
	SDL_Color color{ 255, 255, 255 };
	graphics->LoadTexture("./Ultima_4_512_512.png", "ULTIMA", color);
	if (graphics->GetTexture("ULTIMA") == nullptr) {
		std::cout << "Cannot load ULTIMA spritesheet!\n";
	}

	// TODO: Remove hardcoding first map width and height
	int mapWidth = 1280;
	int mapHeight = 1280;

	// Map resource manager spins up
	cMapResManager mapResManager;

	cGameScreen s{ GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT, 0, 0 };
	cPlayer p{160, 448, 31, "ULTIMA", 16, 10};
	// cPlayer p{1248, 640, 31, "ULTIMA", 16, 10};
	cMonsterOrc o{ 0, 0, 132, "ULTIMA", 2, 10 };

	s.Init(p, mapWidth, mapHeight);
	cGameScene gameScene{ graphics, mapResManager.getMap(), s, p};
	gameScene.registerNPC(&o, "ULTIMA", cMapLocation(256, 640));

	gameScene.run();

	delete graphics;
	graphics = nullptr;

	return 0;
}