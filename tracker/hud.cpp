#include "hud.h"
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

//draw hub
void Hud::draw(SDL_Renderer* renderer) const {

	SDL_Texture* textureBox = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("Dialogue/file").c_str());
	SDL_Rect outputBox = { 190, 100, 500, 253 };
    SDL_RenderCopy(renderer, textureBox, NULL, &outputBox);

    SDL_Texture* textureGirl = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("GirlStop/file").c_str());
    SDL_Rect outputGirl = { 650, 170, 64, 127 };
    SDL_RenderCopy(renderer, textureGirl, NULL, &outputGirl);

    SDL_DestroyTexture(textureBox);
    SDL_DestroyTexture(textureGirl);

	IoMod::getInstance().writeText("a: turn left", 255, 160);
	IoMod::getInstance().writeText("d: turn right", 255, 200);
	IoMod::getInstance().writeText("space: jump", 255, 240);
	IoMod::getInstance().writeText("m: collision detection strategy", 255, 280);

	IoMod::getInstance().writeText("T: locate object", 442, 180);
	IoMod::getInstance().writeText("F1: turn off HUD", 442, 220);
}