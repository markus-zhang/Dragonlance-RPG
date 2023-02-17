#include "cMapResManager.h"
#include "cMap.h"
#include "tinyxml2.h"
#include <sstream>
#include <iostream>

cMapResManager::cMapResManager() {
	// std::vector<int> worldMapData = loadTMX("map01_Solace.tmx", mapColCount, mapRowCount);
	loadMap("map01_Solace.tmx", "ULTIMA");
	loadMap("map02_Merchants.tmx", "MERCHANT1");
	// Create map change locations, hardcode for now
	// TODO: replace hardcoding with a configuration file
	mapRes["ULTIMA"]->getMapChange().push_back(
		cMapChange(
			cMapLocation(160, 448), cMapLocation(160, 288),
			"ULTIMA", "MERCHANT1"
		)
	);
	mapRes["MERCHANT1"]->getMapChange().push_back(
		cMapChange(
			cMapLocation(160, 288), cMapLocation(160, 448),
			"MERCHANT1", "ULTIMA"
		)
	);
}

cMapResManager::~cMapResManager() {
	for (auto& it : mapRes) {
		std::cout << "Releasing resource for map id " << it.first << std::endl;
		delete it.second;
		it.second = nullptr;
	}
}

void cMapResManager::loadMap(const char* path, std::string id) {
	std::vector<int> mapData = loadTMX(path, mapColCount, mapRowCount);
	cMap* world = new cMap(id, mapColCount, mapRowCount, mapData);
	mapRes.emplace(id, world);
}

std::vector<int> cMapResManager::loadTMX(const char* path, int& mapWidth, int& mapHeight) {
	using namespace tinyxml2;
	XMLDocument doc;
	doc.LoadFile(path);
	XMLElement* rootElement = doc.RootElement();
	std::vector<int> v;
	const char* mapText;

	if (rootElement != nullptr) {
		XMLElement* layerElement = rootElement->FirstChildElement("layer");
		const char* width = layerElement->Attribute("width");
		const char* height = layerElement->Attribute("height");
		// Write map width and height
		std::stringstream ss;
		ss << width;
		ss >> mapWidth;
		ss.clear();
		ss << height;
		ss >> mapHeight;
		std::cout << "width: " << mapWidth << "height: " << mapHeight << "\n";

		// Write map data
		XMLElement* mapElement = rootElement->FirstChildElement("layer")->FirstChildElement("data");
		if (mapElement == nullptr) {
			std::cout << "No map pointer!\n";
		}
		else {
			mapText = mapElement->GetText();
			std::string map = std::string(mapText);
			std::istringstream is(map);
			std::string token;
			while (getline(is, token, ',')) {
				v.push_back(std::stoi(token));
			}
		}
	}
	else {
		std::cout << "No root pointer!\n";
	}

	return v;
}