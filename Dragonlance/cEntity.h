#pragma once
#include "Parameters.h"
#include <string>

class cGraphics;
class cGameScreen;

class cEntity {
public:
	cEntity() = default;
	cEntity(int wX, int wY, int ssbi, std::string ssid, int nf, int mvpt):
		worldX(wX), worldY(wY), width(32), height(32),
		spriteSheetBaseIndex(ssbi), spriteSheetId(ssid),
		numFrames(nf), currentFrame(0), movePoint(mvpt),
		animationCounter(ANIMATION_COUNTER), 
		seenPlayer(false) {}

	virtual void updateFrame();
	/*virtual void draw(cGraphics* g, const cGameScreen& s);
	virtual void move(int dir);*/
	virtual void updateAnimationCounter();

public:
	int worldX;
	int worldY;
	int width;
	int height;
	int spriteSheetBaseIndex;
	std::string spriteSheetId;
	// std::unordered_map < std::string, std::vector<int>> animationStartIndex;
	// Given spriteSheetIndex=10, numFrames=5,
	// it says we cycle through sprite 10~14
	int numFrames;
	int currentFrame;
	// Compare movePoint of entity of tile to find 
	int movePoint;
	int animationCounter;
	bool seenPlayer;
};
