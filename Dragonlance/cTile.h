#pragma once

#include <string>

class cEntity;
class cGraphics;

class cTile {
public:
	std::string spriteSheetId;
	int spriteSheetIndex;
	int numTurnsStay;		// Number of turns to get out of the tile, usually 0, 1 for swamp/woods and 2 for forest, etc.
	int minActionPoint;		// Minimum value of action point to get into this tile (fixed for any entity)
	int collisionEvent;		// Event handler when collision occurs (-1 for nothing)
	// cEntity* entityOccupied;	// Pointer to the entity currently
	int worldCol;
	int worldRow;
	int width;
	int height;
	bool blocker;	// FOV blocker?
	bool shadowed;	// Shadowed by a blocker?
	bool isWater;
public:
	cTile() = default;
	// cTile(cTile& t) = delete;
	cTile(std::string spriteSheetId, int spriteSheetIndex, int numTurnsStay, int collisionEvent, int worldCol, int worldRow, int width, int height);
	void draw(cGraphics* g);
	bool visible();
};
