#include "cMerchantController.h"
#include "cMerchant.h"
#include "cItemFactory.h"
#include "cItem.h"
#include "cPlayer.h"

void cMerchantController::initMerchant(
	cMerchant& merchant, cItemFactory& itemFactory, std::string type) {
	/// <comment>
	/// Fill menu for the merchant based on type
	/// </comment>
	
	if (type == "armor") {
		merchant.addItem(itemFactory.createItem("HELMET_IRON"));
		merchant.addItem(itemFactory.createItem("ARMOR_IRON"));
	}
	else if (type == "jewel") {
		merchant.addItem(itemFactory.createItem("AMULET_BLESSED"));
		merchant.addItem(itemFactory.createItem("RING_FIRE"));
	}
}

void cMerchantController::playerBuy(cMerchant& merchant, cPlayer& player, int itemIndex) {
	/// <comment>
	/// itemIndex is the index in merchant.menu
	/// </comment>
	
	int price = (merchant.getMenu())[itemIndex]->getPrice().buy;
	std::string itemID = merchant.getMenu()[itemIndex]->getID();
	if (price <= player.money) {
		player.money -= price;
		// Give player this item
		// If player already has such item, add 1
		// If player does not have such item, create a new dictionary entry
		player.addItem(merchant.getMenu()[itemIndex]);
	}
}