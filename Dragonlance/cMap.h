#pragma once

#include "cTile.h"
#include <vector>
#include <cassert>

class cGameScreen;
class cGraphics;
class cEntity;
struct SDL_Rect;

struct cMapLocation {
	int x;
	int y;
	cMapLocation(int x, int y) : x(x), y(y) {}
};

struct cMapChange {
	cMapLocation sourceLoc;
	cMapLocation destLoc;
	std::string sourceMapID;
	std::string destMapID;
	cMapChange(cMapLocation source, cMapLocation dest, std::string sID, std::string dID) :
		sourceLoc(source), destLoc(dest), sourceMapID(sID), destMapID(dID)
	{}
};

class cMap {
public:
	cMap() = delete;
	cMap(std::string spriteSheetId, int mapWidth, int mapHeight, std::vector<int> tmxData);
	std::vector<std::vector<cTile>>& getMap();
	SDL_Rect getMapPixelDimension();
	void draw(cGraphics* g, const cGameScreen& s);

	int getMapWidth() { return mapWidth; }
	int getMapHeight() { return mapHeight; }
	std::vector<cEntity*>& getNPC() { return npc; }
	std::vector<cMapChange>& getMapChange() { return mapChange; }

	void addNPC(cEntity* e) { 
		assert(e != nullptr);
		npc.push_back(e);
	}
private:
	int mapWidth;	// width
	int mapHeight;	// height
	std::vector<std::vector<cTile>> map;
	std::string spriteSheetId;
	std::vector<cEntity*> npc;
	std::vector<cMapChange> mapChange;

	cTile int2Tile(int index, int worldCol, int worldRow, int width, int height);
};