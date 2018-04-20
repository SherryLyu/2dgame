#include <string>
#include "hud.h"
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

//f1 help menu
void Hud::help(SDL_Renderer* renderer) const {
	SDL_Texture* textureBox = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("Dialogue/file").c_str());
	SDL_Rect outputBox = { 190, 100, 500, 253 };
    SDL_RenderCopy(renderer, textureBox, NULL, &outputBox);

    SDL_Texture* textureGirl = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("GirlStop/file").c_str());
    SDL_Rect outputGirl = { 650, 170, 64, 127 };
    SDL_RenderCopy(renderer, textureGirl, NULL, &outputGirl);

    SDL_DestroyTexture(textureBox);
    SDL_DestroyTexture(textureGirl);

	IoMod::getInstance().writeText("a : turn left", 255, 160);
	IoMod::getInstance().writeText("d : turn right", 255, 200);
	IoMod::getInstance().writeText("space : jump", 255, 240);
	IoMod::getInstance().writeText("m : collision strategy", 255, 280);

	IoMod::getInstance().writeText("t : locate object", 442, 160);
	IoMod::getInstance().writeText("h : hit enemy", 442, 200);
	IoMod::getInstance().writeText("F1 : turn off HUD", 442, 240);
	IoMod::getInstance().writeText("r : restart", 480, 280);
}

//losing sign
void Hud::losing(SDL_Renderer* renderer) const {
	SDL_Texture* textureBox = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("Dialogue/file").c_str());
	SDL_Rect outputBox = { 190, 100, 500, 253 };
    SDL_RenderCopy(renderer, textureBox, NULL, &outputBox);

    SDL_Texture* textureGirl = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("GirlLose/file").c_str());
    SDL_Rect outputGirl = { 550, 160, 121, 129 };
    SDL_RenderCopy(renderer, textureGirl, NULL, &outputGirl);

    SDL_DestroyTexture(textureBox);
    SDL_DestroyTexture(textureGirl);

	IoMod::getInstance().writeText("You have lost all your animals!", 255, 200);
	IoMod::getInstance().writeText("Hit R to restart!", 255, 240);
	
}

//winning sign
void Hud::winning(SDL_Renderer* renderer) const {
	SDL_Texture* textureBox = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("Dialogue/file").c_str());
	SDL_Rect outputBox = { 190, 100, 500, 253 };
    SDL_RenderCopy(renderer, textureBox, NULL, &outputBox);

    SDL_Texture* textureGirl = IMG_LoadTexture(renderer, Gamedata::getInstance().getXmlStr("GirlWin/file").c_str());
    SDL_Rect outputGirl = { 550, 150, 80, 155 };
    SDL_RenderCopy(renderer, textureGirl, NULL, &outputGirl);

    SDL_DestroyTexture(textureBox);
    SDL_DestroyTexture(textureGirl);

	IoMod::getInstance().writeText("You protected all your animals!", 255, 200);
	IoMod::getInstance().writeText("Hit R to restart!", 255, 240);
	
}

//winning sign
void Hud::count(int birdcount, int animalcount) const {
	IoMod::getInstance().writeText("Animals left: " + std::to_string(animalcount), 700, 60);
	IoMod::getInstance().writeText("Birds left: " + std::to_string(birdcount), 730, 90);
}
