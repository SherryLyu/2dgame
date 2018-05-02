//menu page (open by m)
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include "menuEngine.h"
#include "renderContext.h"

MenuEngine::~MenuEngine() { }

MenuEngine::MenuEngine() :
  clock( Clock::getInstance() ),
  renderer( RenderContext::getInstance()->getRenderer() ),
  menu( renderer ),
  lastoption(-1)
{
}


void MenuEngine::draw() const {
  menu.draw();
  SDL_RenderPresent(renderer);
}

void MenuEngine::update(Uint32) {
}

void MenuEngine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_S] ) menu.incrIcon();
        if (keystate[SDL_SCANCODE_W] ) menu.decrIcon();
        if (keystate[SDL_SCANCODE_RETURN] ) {
          menu.chooseOn();
          int option = menu.getOptionNo();
          if ( option == 0 ) done = true;
          if ( option == 1 ) menu.getInputEventLoop();
          if ( option == 2 && lastoption != 2 ){
            menu.setShowControl(true);
            lastoption = 2;
          }
          else if ( option == 2 && lastoption == 2 ){
            menu.setShowControl(false);
            lastoption = -1;
          }
        }
      }
      if(event.type == SDL_KEYUP) {
        menu.chooseOff();
      }
    }
    draw();
  }
}
