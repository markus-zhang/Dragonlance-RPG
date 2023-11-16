#include "cItemFactory.h"
#include "cItem.h"
#include <stdexcept>
#include <vector>

cItemFactory::cItemFactory() {
	loadFactory();
}

cItemFactory::~cItemFactory() {
	for (auto o : ownedItems) {
		delete o;
		o = nullptr;
	}
}

cItem* cItemFactory::createItem(std::string id) {
	for (auto item : itemFactory) {
		if (item.second.getID() == id) {
			cItem* i = new cItem(
				item.second.getID(), item.second.getName(),
				item.second.getSlotID(), item.second.isStackable()
			);
			ownedItems.push_back(i);
			return i;
		}
	}
	throw std::invalid_argument("Invalid item id value");
}

void cItemFactory::loadFactory() {
	// All hardcoded
	cItem helmetIron("HELMET_IRON", "Iron helmet", 1, false);
	helmetIron.setPrice(price{ 25, 100 });

	cItem amuletBlessing("AMULET_BLESSED", "Blessed amulet", 2, false);
	amuletBlessing.setPrice(price{ 50, 250 });

	cItem armorIron("ARMOR_IRON", "Iron armor", 4, false);
	armorIron.setPrice(price{ 200, 1000 });

	cItem ringFire("RING_FIRE", "Ring of fire", 8, false);
	ringFire.setPrice(price{ 500, 1500 });

	cItem ringProtection("RING_PROTECTION", "Ring of protection", 16, false);
	ringProtection.setPrice(price{ 500, 1800 });

	itemFactory["HELMET_IRON"] = helmetIron;
	itemFactory["AMULET_BLESSED"] = amuletBlessing;
	itemFactory["ARMOR_IRON"] = armorIron;
	itemFactory["RING_FIRE"] = ringFire;
	itemFactory["RING_PROTECTION"] = ringProtection;
}