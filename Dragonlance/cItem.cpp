#include "cItem.h"
#include "Parameters.h"
#include <stdexcept>

cItem::cItem(std::string id, std::string name, int slotID, bool stackable) {
	if (!assessSlotID(slotID)) {
		throw std::invalid_argument("Invalid slotID value");
	}
	name = id;
	displayName = name;
	slotID = slotID;
	stackable = stackable;
	quantity = 1;
	p.buy = 0;
	p.sell = 0;
}

bool cItem::assessSlotID(int slotID) {
	return (slotID == HEAD || slotID == NECK || slotID == BODY || slotID == RING_LEFT || slotID == RING_RIGHT);
}

bool cItem::isStackable() {
	return stackable;
}

std::string cItem::getID() {
	return id;
}

std::string cItem::getName() {
	return displayName;
}

int cItem::getQuantity() {
	return quantity;
}

price cItem::getPrice() {
	return p;
}

int cItem::getSlotID() {
	return slotID;
}

void cItem::setPrice(price p) {
	p = p;
}

void cItem::addQuantity(int q) {
	quantity += q;
}