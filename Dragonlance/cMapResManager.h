#pragma once
#include <unordered_map>

/*
* Load maps as resource (pointers)
* Note that this class owns all of its resources so it needs to free them
*/

class cMap;

class cMapResManager {
public:
	cMapResManager();
	~cMapResManager();

	void loadMap(const char* path, std::string id);
	cMap* getMapByID(std::string id) {
		return mapRes[id];
	}
	std::unordered_map<std::string, cMap*>* getMap() {
		return &mapRes;
	}

private:
	std::unordered_map<std::string, cMap*> mapRes;
	int mapColCount;
	int mapRowCount;
	std::vector<int> loadTMX(const char* path, int& mapWidth, int& mapHeight);
};