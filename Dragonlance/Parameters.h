#pragma once

#define DEBUG false

#define FPS 30
#define MILLISECONDS_PER_FRAME 1000 / FPS
// Play animation every 15 frames
#define ANIMATION_COUNTER 15

#define TILE_WIDTH 32
#define TILE_HEIGHT 32
// #define X_TILES 40
// #define Y_TILES 40

#define GAME_SCREEN_WIDTH 768
#define GAME_SCREEN_HEIGHT 640

// map width cannot be smaller than 160, otherwise vector access violation
#define MIN_WIDTH_SCROLL 160
#define MIN_HEIGHT_SCROLL 160
// Should not hardcode MAX values
// MAX values should always be mapWidth - MIN_WIDTH_SCROLL
// So that we leave 5 tiles for both sides
// #define MAX_WIDTH_SCROLL 1120
// #define MAX_HEIGHT_SCROLL 1120

#define EAST 1
#define SOUTH 2
#define WEST 3
#define NORTH 4

// For item slot
#define HEAD 1
#define NECK 2
#define BODY 4
#define RING_LEFT 8
#define RING_RIGHT 16