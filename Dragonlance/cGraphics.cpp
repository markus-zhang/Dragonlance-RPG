#include "cGraphics.h"
#include "cMap.h"
#include "cTile.h"
#include "cEntity.h"
#include "DebugTool.h"
#include "SDL_FontCache.h"

cGraphics::cGraphics(int wndWidth, int wndHeight, std::string wndCaption)
{
	// Tell SDL to intialize its video component 
	SDL_Init(SDL_INIT_VIDEO);

	// Initialize the True Type Font library
	TTF_Init();

	//SDL 2.0 code to create a window
	m_Window = SDL_CreateWindow(wndCaption.c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		wndWidth, wndHeight,
		SDL_WINDOW_RESIZABLE);
	//SDL 2.0 code to create renderer for window
	m_Renderer = SDL_CreateRenderer(m_Window, -1,
		0);
	
	// Setting up viewport
	/*SDL_Rect topLeftViewport{ 0, 0, 800, 640 };
	SDL_RenderSetViewport(m_Renderer, &topLeftViewport);*/

	//SDL 2.0 code to initiate renderer color
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0xFF);
	//SDL 2.0 code to initiate PNG loading
	IMG_Init(IMG_INIT_PNG);

	// Loading font
	font = FC_CreateFont();
	FC_LoadFont(font, m_Renderer, "martian.fon", 40, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
	if (font == nullptr) {
		std::cout << "Font doesn't work!" << std::endl;
	}
}

cGraphics::~cGraphics()
{
	SDL_DestroyWindow(m_Window);
	CloseImage();

	// Shutdown the True Type Font library
	TTF_Quit();

	//SDL 2.0 code to shutdown PNG loading facility
	IMG_Quit();

	// Tell SDL to shutdown and free any resources it was using. 
	SDL_Quit();

	// Release font
	delete font;
	font = nullptr;
}

void cGraphics::LoadTexture(const std::string& textureFilename, const std::string& id, const SDL_Color& color) {
	SDL_Surface* m_Temp = IMG_Load(textureFilename.c_str());
	//Set color key
	SDL_SetColorKey(m_Temp, SDL_TRUE,
		SDL_MapRGB(m_Temp->format, color.r, color.g, color.b));

	SDL_Texture* texture = SDL_CreateTextureFromSurface(
		m_Renderer, m_Temp);

	m_Texture.insert({ id, texture });

	SDL_FreeSurface(m_Temp);
}

void cGraphics::RenderTexture(const std::string& textureId, const SDL_Rect& srcrect, const SDL_Rect& dstrect)
{
	SDL_RenderCopy(m_Renderer, m_Texture[textureId], &srcrect, &dstrect);
}

void cGraphics::RenderRect(int x, int y, int width, int height,
	int red, int green, int blue, int alpha) {
	SDL_Rect rect = { x, y, width, height };
	SDL_SetRenderDrawColor(m_Renderer, red, green, blue, alpha);
	SDL_SetRenderDrawBlendMode(m_Renderer, SDL_BLENDMODE_BLEND);
	//SDL_RenderDrawRect(m_Renderer, &rect);
	SDL_RenderFillRect(m_Renderer, &rect);
	SDL_RenderPresent(m_Renderer);
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
}

void cGraphics::SetAlpha(std::string textureId, Uint8 alpha) {
	SDL_SetTextureAlphaMod(m_Texture[textureId], alpha);
}

void cGraphics::ClearScreen() {
	SDL_RenderClear(m_Renderer);
}

void cGraphics::Show() {
	SDL_RenderPresent(m_Renderer);
}

void cGraphics::CloseImage() {
	for (auto& kv : m_Texture) {
		SDL_DestroyTexture(kv.second);
	}
	SDL_DestroyRenderer(m_Renderer);
}

/*
void cGraphics::LoadText(std::string text, int size, SDL_Color color) {
	//Load text into a surface
	TTF_Font* m_Font = TTF_OpenFont("ARIAL.ttf", size);
	SDL_Surface* m_Temp = TTF_RenderText_Solid(m_Font,
		text.c_str(), color);
	m_Texture = SDL_CreateTextureFromSurface(m_Renderer, m_Temp);

	SDL_FreeSurface(m_Temp);
}

void cGraphics::RenderText(std::string text, int size, int r, int g, int b,
	int srcX, int srcY)
{
	//Load text into a surface
	TTF_Font* m_Font = TTF_OpenFont("ARIAL.ttf", size);
	SDL_Color m_Color = { r, g, b };
	SDL_Surface* m_Temp = TTF_RenderText_Solid(m_Font,
		text.c_str(), m_Color);
	int width = m_Temp->w;
	int height = m_Temp->h;
	m_Texture = SDL_CreateTextureFromSurface(m_Renderer, m_Temp);
	SDL_Rect g_DesRect = { srcX, srcY, width, height };
	SDL_RenderCopy(m_Renderer, m_Texture, NULL, &g_DesRect);
	SDL_FreeSurface(m_Temp);
	TTF_CloseFont(m_Font);
}
*/

void cGraphics::RenderText(int screenX, int screenY, const char* text, const char* id) {
	// FC_Draw(font, m_Renderer, screenX, screenY, text, id);
	FC_DrawBox(font, m_Renderer, SDL_Rect{ 0, 0, 200, 200 }, text);
}

void cGraphics::RenderTile(const cTile& t) {
	SDL_Rect source = indexToSourceRect(t.spriteSheetIndex, t.width, t.height, 512, 512);
	SDL_Rect dest{ t.worldCol, t.worldRow, t.width, t.height };
	// TODO: Refactor the hardcoded ULTIMA, need to refactor code that calls this too
	RenderTexture("ULTIMA", source, dest);
}

void cGraphics::RenderTileWithBias(const cTile& t, const int& biasCol, const int& biasRow) {
	SDL_Rect source = indexToSourceRect(t.spriteSheetIndex, t.width, t.height, 512, 512);
	SDL_Rect dest{ t.worldCol + biasCol, t.worldRow + biasRow, t.width, t.height };
	// TODO: Refactor the hardcoded ULTIMA, need to refactor code that calls this too
	RenderTexture("ULTIMA", source, dest);
}

void cGraphics::RenderEntityWithBias(const cEntity& e, const int& biasCol, const int& biasRow) {
	SDL_Rect source = indexToSourceRect(e.spriteSheetBaseIndex + e.currentFrame, e.width, e.height, 512, 512);
	SDL_Rect dest{ e.worldX + biasCol, e.worldY + biasRow, e.width, e.height };
	// TODO: Refactor the hardcoded ULTIMA, need to refactor code that calls this too
	RenderTexture("ULTIMA", source, dest);
}

SDL_Rect cGraphics::indexToSourceRect(
	int spriteSheetIndex,
	int tileWidth, int tileHeight,
	int spritesheetWidth, int spritesheetHeight) {
	// 0, 32, 32, 512, 512 means the first sprite which has size of 32*32
	// the whole sheet is 512*512
	int numElementPerRow = spritesheetWidth / tileWidth;
	int numElementPerCol = spritesheetHeight / tileHeight;

	// e.g. it's a 16 * 16 tileset, so tile 27 should be in row 1, col 11
	// remember everything starts from 0!
	int y = (spriteSheetIndex / numElementPerRow) * tileWidth;
	int x = (spriteSheetIndex - (y / tileWidth) * numElementPerRow) * tileHeight;

	return SDL_Rect{ x, y, tileWidth, tileHeight };
}