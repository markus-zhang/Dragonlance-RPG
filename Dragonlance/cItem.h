#pragma once
#include <string>

struct price {
	// sell usually < buy
	// sell < 0 then do not buy back
	int sell;
	int buy;
};

class cItem {
private:
	std::string name;
	std::string description;
	// Which slot (arm/head/etc.) can equip, 0 for none (e.g. consumables)
	int slotId;
	bool stackable;
	int quantity;
	price p;
	// TODO: Add effect class here
public:
	cItem() = default;
	cItem(std::string name, std::string des, int slotId, bool stackable);
	void setPrice(price p);

};