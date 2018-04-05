#include "hud.h"
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

void Hud::draw(SDL_Renderer* renderer) const {

	SDL_Texture* textureBox = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("Dialogue/file").c_str());
	SDL_Rect outputBox = { 200, 110, 500, 253 };
    SDL_RenderCopy(renderer, textureBox, NULL, &outputBox);

    SDL_Texture* textureGirl = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("GirlStop/file").c_str());
    SDL_Rect outputGirl = { 560, 180, 64, 127 };
    SDL_RenderCopy(renderer, textureGirl, NULL, &outputGirl);

	IoMod::getInstance().writeText("a: turn left", 300, 160);
	IoMod::getInstance().writeText("d: turn right", 300, 200);
	IoMod::getInstance().writeText("space: jump", 300, 240);
	IoMod::getInstance().writeText("F1: turn off HUD", 300, 280);
}