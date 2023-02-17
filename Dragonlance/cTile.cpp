#include "cTile.h"
#include "cGraphics.h"

cTile::cTile(std::string spriteSheetId, int spriteSheetIndex, int numTurnsStay, int collisionEvent, int worldCol, int worldRow, int width, int height) {
	this->spriteSheetId = spriteSheetId;
	this->spriteSheetIndex = spriteSheetIndex;
	this->numTurnsStay = numTurnsStay;
	this->collisionEvent = collisionEvent;
	this->worldCol = worldCol;
	this->worldRow = worldRow;
	// this->entityOccupied = nullptr;
	this->width = width;
	this->height = height;
	this->shadowed = false;
	// hardcoded tile for blocker
	if (spriteSheetIndex >= 7 && spriteSheetIndex <= 11) {
		this->blocker = true;
	}
	else {
		this->blocker = false;
	}
	// hardcoded tile for ai controller
	// 0 is empty so should not be water
	if (spriteSheetIndex <= 2 && spriteSheetIndex >= 1) {
		this->isWater = true;
	}
	else {
		this->isWater = false;
	}
	// hardcoded tile for ai controller
	switch (spriteSheetIndex) {
	case 3:
	case 4:
		minActionPoint = 0;
		break;
	case 5:
		minActionPoint = 5;
		break;
	case 6:
		minActionPoint = 10;
		break;
	case 7:
	case 8:
	case 9:
		minActionPoint = 15;
		break;
	default:
		minActionPoint = 0;
	}
}

void cTile::draw(cGraphics* g) {
	g->RenderTile(*this);
}

bool cTile::visible() {
	return (blocker == false && shadowed && false);
}