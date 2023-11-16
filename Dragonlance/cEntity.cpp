#include "cEntity.h"
#include "cGameScreen.h"
#include "cGraphics.h"
#include "cItem.h"

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

void cEntity::addItem(cItem* i) {
	/// <summary>Add an item into the inventory.
	/// Assumptions:
	///		- Stackable items will never overflow 64-bit integer
	///		- Stackable items are removed when quatity reaches 0
	///		- Non-stackable items never degrade
	///		- Non-stackable items do not duplicate
	/// <param name='i'>A pointer to a cItem object, for polymorphism</param>
	/// </summary>
	for (auto item : inventory) {
		if (item->getName() == i->getName()) {
			if (item->isStackable()) {
				item->addQuantity(i->getQuantity());
			}
		}
		else {
			inventory.push_back(i);
		}
	}
}