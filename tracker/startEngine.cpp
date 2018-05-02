//the start menu of game
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include "sprite.h"
#include "gamedata.h"
#include "startEngine.h"
#include "numberdata.h"
#include "renderContext.h"

StartEngine::~StartEngine() { }

StartEngine::StartEngine() :
  clock( Clock::getInstance() ),
  renderer( RenderContext::getInstance()->getRenderer() ),
  start( renderer ),
  lastoption(-1)
{
}

void StartEngine::draw() const {
  start.draw();
  SDL_RenderPresent(renderer);
}

void StartEngine::update(Uint32) {
}

bool StartEngine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { return false; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE]) {
          return false;
        }
        if (keystate[SDL_SCANCODE_S] ) start.incrIcon();
        if (keystate[SDL_SCANCODE_W] ) start.decrIcon();
        if (keystate[SDL_SCANCODE_RETURN] ) {
          start.chooseOn();
          int option = start.getOptionNo();
          if ( option == 0 ) return true;
          if ( option == 1 ) start.getInputEventLoop();
          if ( option == 2 && lastoption != 2 ){
            start.setShowControl(true);
            lastoption = 2;
          }
          else if ( option == 2 && lastoption == 2 ){
            start.setShowControl(false);
            lastoption = -1;
          }
        }
      }
      if(event.type == SDL_KEYUP) {
        start.chooseOff();
      }
    }
    draw();
  }
  return false;
}
