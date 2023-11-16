#pragma once
#include <string>

struct price {
	// sell usually < buy
	// sell < 0 then do not buy back
	int sell;
	int buy;
	price() : sell(0), buy(0) {}
	price(int s, int b) : sell(s), buy(b) {}
};

class cItem {
private:
	std::string id;
	std::string displayName;
	// Which slot (arm/head/etc.) can equip, 0 for none (e.g. consumables)
	int slotID;
	bool stackable;
	int quantity;
	price p;
	// TODO: Add effect class here

	bool assessSlotID(int slotID);
public:
	cItem() = default;
	cItem(std::string id, std::string name, int slotID, bool stackable);
	bool isStackable();
	std::string getID();
	std::string getName();
	int getSlotID();
	int getQuantity();
	price getPrice();
	void setPrice(price p);
	void addQuantity(int q);

};