#pragma once
#include "cEntity.h"
#include <vector>
#include <string>

/// <summary>
/// cMerchant is mostly a data class
/// cMerchantController is the system class
/// cMerchant contains the following data:
///		- Everything cEntity has
///		- Merchant's name
///		- A vector of items (Merchant does not depelete stock)
///		- Amount of gold available to buy back player's items
/// </summary>

class cItem;

class cMerchant : public cEntity {
public:
	cMerchant() = default;
	cMerchant(int wX, int wY, int ssbi, std::string ssid, int nf, int mvpt, std::string type) :
		cEntity(wX, wY, ssbi, ssid, nf, mvpt) {
		// Initialize menu based on merchant type

		// Initialize money
		// money = 100d10
	}

	/// <comment>
	/// All functionalities live in the controller class
	/// </comment>
	// void init(std::string type);
	// void updateMenu(cItem* i);

	std::vector<cItem*>& getMenu();

public:
	std::string name;
	std::vector<cItem*> menu;
	int money;
};