#pragma once
#include <string>
#include <unordered_map>

class cItem;

class cItemFactory {
/// <summary>
/// cItemFactory should own EVERY item in the game
/// It is essentially a resource management class
/// </summary>
private:
	std::unordered_map<std::string, cItem> itemFactory;
	std::vector<cItem* > ownedItems;
public:
	cItemFactory();
	~cItemFactory();
	cItem* createItem(std::string id);
	void loadFactory();
};