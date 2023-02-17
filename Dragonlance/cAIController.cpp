#include "cAIController.h"
#include "cGameScene.h"
#include "cMap.h"
#include <iostream>
#include <cassert>

void cAIController::markSeenPlayer(cEntity* e) {
	// Usually when player dies and revivies or exits map
	e->seenPlayer = true;
}

void cAIController::unmarkSeenPlayer(cEntity* e) {
	// Usually when player dies and revivies or exits map
	e->seenPlayer = false;
}

void cAIController::think(cEntity* e, cGameScene* s) {
	// If player can see entity then entity can see player
	if (!e->seenPlayer) {
		if (s->getCurrentMap()->getMap()[e->worldX / TILE_WIDTH][e->worldY / TILE_HEIGHT].shadowed == false) {
			markSeenPlayer(e);
		}
	}

	// If haven't seen player just move randomly
	if (!e->seenPlayer) {
		moveRandomly(e, s->getCurrentMap());
	}
	// If seen player then always chase player if in same 
	else {
		chasePlayer(e, s->getPlayer());
	}
}

// What to do if we want to unmarkSeenPlayer() when player uses a cloaking artifact?
// Apparently using an item calls some code from itemController or effectController
// which needs to know the state of each entity


void cAIController::moveRandomly(cEntity* e, cMap* m) {
	int max = 4;
	int min = 1;
	// Generate one direction randomly (don't care if it's movable in that direction)
	int dir = rand() % (max - min + 1) + min;
	moveEntity(e, dir, m);
}

void cAIController::moveEntity(cEntity* e, int dir, cMap* m) {
	switch (dir) {
	case EAST:
		if (canMoveDirection(e, dir, m)) {
			e->worldX += TILE_WIDTH;
		}
		break;
	case SOUTH:
		if (canMoveDirection(e, dir, m)) {
			e->worldY += TILE_HEIGHT;
		}
		break;
	case WEST:
		if (canMoveDirection(e, dir, m)) {
			e->worldX -= TILE_WIDTH;
		}
		break;
	case NORTH:
		if (canMoveDirection(e, dir, m)) {
			e->worldY -= TILE_HEIGHT;
		}
		break;
	default:
		break;
	}
}

bool cAIController::canMoveDirection(cEntity* e, int dir, cMap* m) {
	assert(dir >= EAST && dir <= NORTH);
	int colIndex = e->worldX / TILE_WIDTH;
	int rowIndex = e->worldY / TILE_HEIGHT;
	cTile target;

	switch (dir) {
	case EAST: 
		if (colIndex == m->getMapWidth() - 1) { return false; }
		target = m->getMap()[rowIndex][colIndex+1];
		break;
	case SOUTH:
		if (rowIndex == m->getMapHeight() - 1) { return false; }
		target = m->getMap()[rowIndex+1][colIndex];
		break;
	case WEST:
		if (colIndex == 0) { return false; }
		target = m->getMap()[rowIndex][colIndex-1];
		break;
	case NORTH:
		if (rowIndex == 0) { return false; }
		target = m->getMap()[rowIndex-1][colIndex];
		break;
	default:
		break;
	}

	if (target.isWater) {
		// TODO: add code for ships
		std::cout << "Is water" << std::endl;
		return false;
	}
	// std::cout << "Move Point" << e->movePoint << " vs " << target.minActionPoint << std::endl;
	return (e->movePoint >= target.minActionPoint);
}

void cAIController::chasePlayer(cEntity* e, cPlayer* p) {

}

bool cAIController::inSameMap(cEntity* e, cPlayer* p) {
	return true;
}