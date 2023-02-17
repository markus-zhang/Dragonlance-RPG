#include "cEntity.h"
#include "cGameScreen.h"
#include "cGraphics.h"

void cEntity::updateFrame() {
	if (animationCounter == 0) {
		if (currentFrame < numFrames) {
			currentFrame++;
		}
		else {
			currentFrame = 0;
		}
		animationCounter = ANIMATION_COUNTER;
	}
	else {
		animationCounter--;
	}
}

void cEntity::updateAnimationCounter() {
	animationCounter--;
}

