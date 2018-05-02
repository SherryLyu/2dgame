#include <iostream>
#include <string>
#include "gamedata.h"
#include <SDL2/SDL.h>

class Hud {
public:
	Hud(SDL_Renderer*);
	void draw(std::string) const;
	void help() const;
	void losing() const;
	void winning() const;
	void godmode() const;
	void count(int, int) const;
private:
	SDL_Renderer* renderer;
 	Gamedata& gdata;
  	SDL_Rect frame;
	SDL_Rect girlHelp;
	SDL_Rect girlLose;
	SDL_Rect girlWin;
};


