#include <iostream>
#include <SDL2/SDL.h>

class Hud {
public:
	void help(SDL_Renderer*) const;
	void losing(SDL_Renderer*) const;
	void winning(SDL_Renderer*) const;
	void count(int, int) const;
};


