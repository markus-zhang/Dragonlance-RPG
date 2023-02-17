#pragma once
/*
* Each scene matches a part of the game:
*	- Main menu
*	- Main Game Scene (World map/Town/Dungeon/etc.)
*	- Status Bar
*	- Inventory
* 
* The whole application contains 3 scenes (with their respective cScreen shown on screen)
* But each scene slot may switch to other scenes, so it's not always the same 3 scenes
*/

class cGraphics;
class cScreen;

class cScene {
public:
	cScene() : renderer(nullptr) {}
	cScene(cGraphics* g) : renderer(g) {}
	~cScene() {
		renderer = nullptr;	// non-owning pointer
	}
	virtual void draw() = 0;
	virtual void run() = 0;
protected:
	// All scenes share one renderer (not owned by any scene)
	cGraphics* renderer;
};