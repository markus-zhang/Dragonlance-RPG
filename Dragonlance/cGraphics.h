/*
	cGraphics is the rendering engine.
	It should own the following:
		- a SDL_Windows instance (Use it to paint on)
		- a SDL_Renderer instance (Use it to paint)
		- all SDL_Texture resources
	It basically only cares about:
		- which SDL_Texture it should draw
		- where to draw
		- should I draw the whole texture or just part?
		- should I transform the texture, etc.
*/

#pragma once

//SDL 2.0 code for libraries
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_TTF.lib")

#include <string>
#include <unordered_map>
#include "SDL.h"     
#include "SDL_TTF.h" 
#include "SDL_image.h"

class cTile;
class cMap;
struct FC_Font;
class cEntity;

class cGraphics
{
public:
	cGraphics(int wndWidth, int wndHeight, std::string wndCaption);
	~cGraphics();

	//SDL 2.0 code to load a texture
	//Note that text and image share the same m_Texture
	//So either this object is for a text, or it's for an image
	//YOU CANNOT MIX BOTH!
	void LoadTexture(const std::string& textureFilename, const std::string& id, const SDL_Color& color);

	//SDL 2.0 code to render a texture at (x, y)
	void RenderTexture(const std::string& textureId, const SDL_Rect& srcrect, const SDL_Rect& dstrect);

	void RenderRect(int x, int y, int width, int height,
		int red, int green, int blue, int alpha);

	void SetAlpha(std::string textureId, Uint8 alpha);

	void ClearScreen();

	//Step 1: LoadTexture or LoadText
	//Step 2: RenderTexture
	//Step 3: Show()
	void Show();

	void CloseImage();

	// For font I decided to use the alphabet in sprite sheet
	// Next two functions are not useful anymore
	/*
	void LoadText(std::string text, int size, SDL_Color color);

	void RenderText(std::string text, int size, int r, int g, int b,
		int srcX, int srcY);
	*/
	void RenderText(int screenCol, int screenRow, const char* text, const char* id);

	SDL_Texture* GetTexture(std::string textureId) { return m_Texture[textureId]; }
	SDL_Renderer* GetRenderer() { return m_Renderer; }

	// Game-related Rendering Functions
	void RenderTile(const cTile& t);
	void RenderTileWithBias(const cTile& t, const int& biasCol, const int& biasRow);
	void RenderEntityWithBias(const cEntity& e, const int& biasCol, const int& biasRow);

private:
	SDL_Window* m_Window;										//1st, load image to this
	std::unordered_map<std::string, SDL_Texture*> m_Texture;	//2nd, make texture from surface
	SDL_Renderer* m_Renderer;									//3rd, renderer the texture on screen
	FC_Font* font;

	// Utility Functions
	SDL_Rect indexToSourceRect(
		int spriteSheetIndex,
		int tileWidth, int tileHeight,
		int spritesheetWidth, int spritesheetHeight);
};