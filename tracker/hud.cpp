#include "hud.h"
#include "ioMod.h"
#include "gamedata.h"
#include "renderContext.h"

Hud::Hud(SDL_Renderer* rend) :
	renderer(rend), 
	gdata(Gamedata::getInstance()),
	frame( { gdata.getXmlInt("Dialogue/startLocX"),
             gdata.getXmlInt("Dialogue/startLocY"),
			 gdata.getXmlInt("Dialogue/width"),
			 gdata.getXmlInt("Dialogue/height")}
	),
	girlHelp( { gdata.getXmlInt("GirlStop/startLocX"),
				gdata.getXmlInt("GirlStop/startLocY"),
				gdata.getXmlInt("GirlStop/width"),
				gdata.getXmlInt("GirlStop/height")}
  	),
  	girlLose( { gdata.getXmlInt("GirlLose/startLocX"),
				gdata.getXmlInt("GirlLose/startLocY"),
				gdata.getXmlInt("GirlLose/width"),
				gdata.getXmlInt("GirlLose/height")}
  	),
  	girlWin( {  gdata.getXmlInt("GirlWin/startLocX"),
				gdata.getXmlInt("GirlWin/startLocY"),
				gdata.getXmlInt("GirlWin/width"),
				gdata.getXmlInt("GirlWin/height")}
  	)
{ }

void Hud::draw(std::string request) const {
	SDL_Texture* textureBox = IMG_LoadTexture(renderer, 
		gdata.getXmlStr("Dialogue/file").c_str());
	SDL_Rect outputBox = { frame.x, frame.y, frame.w, frame.h };
	SDL_RenderCopy(renderer, textureBox, NULL, &outputBox);

	SDL_Texture* textureGirl;
	SDL_Rect outputGirl;
	if(request == "help"){
		textureGirl = IMG_LoadTexture(renderer, 
			gdata.getXmlStr("GirlStop/file").c_str());
		outputGirl = { girlHelp.x, girlHelp.y, girlHelp.w, girlHelp.h };
		help();
	}else if(request == "lose"){
		textureGirl = IMG_LoadTexture(renderer, 
			gdata.getXmlStr("GirlLose/file").c_str());
		outputGirl = { girlLose.x, girlLose.y, girlLose.w, girlLose.h };
		losing();
	}else if(request == "win"){
		textureGirl = IMG_LoadTexture(renderer, 
			gdata.getXmlStr("GirlWin/file").c_str());
		outputGirl = { girlWin.x, girlWin.y, girlWin.w, girlWin.h };
		winning();
	}
	SDL_RenderCopy(renderer, textureGirl, NULL, &outputGirl);

	SDL_DestroyTexture(textureBox);
    SDL_DestroyTexture(textureGirl);

}
//f1 help menu
void Hud::help() const {

	IoMod::getInstance().writeText("a : turn left", 255, 160);
	IoMod::getInstance().writeText("d : turn right", 255, 200);
	IoMod::getInstance().writeText("space : jump", 255, 240);
	IoMod::getInstance().writeText("m : open menu", 255, 280);

	IoMod::getInstance().writeText("h : hit enemy", 425, 160);
	IoMod::getInstance().writeText("r : restart", 425, 200);
	IoMod::getInstance().writeText("g : turn on/off god mode", 425, 240);
	IoMod::getInstance().writeText("F1 : turn on/off HUD", 425, 280);
}

//losing sign
void Hud::losing() const {

	IoMod::getInstance().writeText("You have lost all your animals!", 255, 200);
	IoMod::getInstance().writeText("Hit R to restart!", 255, 240);
	
}

//winning sign
void Hud::winning() const {

	IoMod::getInstance().writeText("You protected all your animals!", 255, 200);
	IoMod::getInstance().writeText("Hit R to restart!", 255, 240);
	
}

//g god mode 
void Hud::godmode() const {

	IoMod::getInstance().writeText("Hey! God mode is on!", 300, 30);
	IoMod::getInstance().writeText("Animals won't be catched!", 300, 60);
	IoMod::getInstance().writeText("Go kill all the birds!", 300, 90);
}

//count of animals and birds
void Hud::count(int birdcount, int animalcount) const {
	IoMod::getInstance().writeText("Animals left: " + std::to_string(animalcount), 700, 60);
	IoMod::getInstance().writeText("Birds left: " + std::to_string(birdcount), 730, 90);
}

