#pragma once

#include "cScene.h"
#include "cGameScreen.h"
#include "cPlayer.h"
#include "cAIController.h"
#include <vector>
#include <unordered_map>

class cMap;
class cEntity;
struct cMapLocation;

class cGameScene : public cScene {
public:
	cGameScene() = default;
	cGameScene(cGraphics* g, std::unordered_map<std::string, cMap*>* m, cGameScreen s, cPlayer pe) : cScene(g) {
		SDL_assert(m->size() != 0);
		SDL_assert(g != nullptr);
		map = m;
		screen = s;
		playerEntity = pe;
		// Initial map is the world map
		currentMap = (*map)["ULTIMA"];
		SDL_assert(currentMap != nullptr);
		shadowCastingExperimentalFOV();
	}
	~cGameScene() {
	}

	cMap* getMap(std::string id) { return (*map)[id]; }
	cMap* getCurrentMap() { return currentMap; }
	cGameScreen& getGameScreen() { return screen; }
	cPlayer* getPlayer() { return &playerEntity; }
	void draw() override;
	void updatePlayer(int dir);
	void drawPlayer();
	void drawEntity(const cEntity& e);
	void shadowCastingEnhancedFOV();
	void shadowCastingExperimentalFOV();
	bool canScroll(int dir);
	void scroll(int dir);

	// TODO: replace cMap* with a reference to unordered_map<std::string, cMap*>
	// TODO: from the resource management class
	void registerNPC(cEntity* e, std::string mapID, cMapLocation l);

	void changeMap();

	void run() override;

private:
	// should not own map, map is owned by resource manager
	// map should eventually be a pointer to an unordered_map
	std::unordered_map<std::string, cMap*>* map;
	// cMap* map;
	// many objects live in different maps, use currentMap to tell
	cMap* currentMap;
	cGameScreen screen;
	cPlayer playerEntity;
	// TODO: npc should follow map
	// TODO: use std::unordered_map<string, std::vector<cEntity*>
	// TODO: or move npc to map (probably a better idea)
	// std::vector<cEntity*> npc;

	cAIController ai;
};