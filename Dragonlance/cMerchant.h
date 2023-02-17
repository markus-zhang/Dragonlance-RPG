#pragma once
#include "cEntity.h"
#include <unordered_map>
#include <string>

struct price {
	// sell usually < buy
	// sell < 0 then do not buy back
	int sell;
	int buy;
};

class cMerchant : public cEntity {
public:
	cMerchant() = default;
	cMerchant(int wX, int wY, int ssbi, std::string ssid, int nf, int mvpt) :
		cEntity(wX, wY, ssbi, ssid, nf, mvpt) {
		// Initialize menu

		// Initialize money
		// money = 100d10
	}

	std::unordered_map<std::string, price>& getMenu() { return menu; }
	// For both inserting and updating
	void updateMenu(std::string id, price p);
	void removeItem(std::string id);
	void playerBuy(std::string id);
	void playerSell(std::string id);

public:
	std::string name;
	std::unordered_map<std::string, price> menu;
	int money;
};