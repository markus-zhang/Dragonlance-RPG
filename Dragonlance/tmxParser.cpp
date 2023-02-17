//#include "tinyxml2.h"
//#include <iostream>
//
//using namespace tinyxml2;
//
//int main(int argc, char* argv[]) {
//	XMLDocument doc;
//	const char* path = "map01_Solace.tmx";
//	doc.LoadFile(path);
//	XMLElement* rootElement = doc.RootElement();
//
//	if (rootElement != nullptr) {
//		XMLElement* mapElement = rootElement->FirstChildElement("layer")->FirstChildElement("data");
//		if (mapElement == nullptr) {
//			std::cout << "No map pointer!\n";
//		}
//		else {
//			std::cout << "No map pointer!\n";
//		}
//	}
//	else {
//		std::cout << "No root pointer!\n";
//	}
//
//	return 0;
//}