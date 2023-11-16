#pragma once
#include <string>

/// <summary>
/// The controller class for merchants
///		- Initiate merchants
///		- Update merchants
///		- Dealing with transactions between player and merchants
/// </summary>

class cMerchant;
class cPlayer;
class cItemFactory;

class cMerchantController {
	void initMerchant(
		cMerchant& merchant, cItemFactory& itemFactory, std::string type);
	void playerBuy(cMerchant& merchant, cPlayer& player, int itemIndex);
};