#include "cMerchant.h"
#include "cItemFactory.h"
#include "cItem.h"

std::vector<cItem*>& cMerchant::getMenu() {
	return menu;
}