#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "twowaymultisprite.h"
#include "smartSprite.h"
#include "subjectSprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"

Engine::~Engine() { 
  for(size_t i = 0; i < chicken.size(); ++i){
    delete chicken[i];
  }
  for(size_t j = 0; j < girl.size(); ++j){
    delete girl[j];
  }
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  cloud("cloud", Gamedata::getInstance().getXmlInt("cloud/factor") ),
  tower("tower", Gamedata::getInstance().getXmlInt("tower/factor") ),
  tree("tree", Gamedata::getInstance().getXmlInt("tree/factor") ),
  ground("ground", Gamedata::getInstance().getXmlInt("ground/factor") ),
  viewport( Viewport::getInstance() ),
  //use initialization list 
  chicken({new Sprite("Chicken"), new Sprite("Chicken"), new Sprite("Chicken"),
    new Sprite("Chicken"), new Sprite("Chicken"), new Sprite("Chicken"), new Sprite("Chicken")}),
  girl({new TwowaymultiSprite("Girl")}),
  drops(),
  girlPlayer(new SubjectSprite("Girl")),
  strategies(),
  currentStrategy(0),
  currentSprite(0),
  collision(false),
  makeVideo( false )
{
  int n = Gamedata::getInstance().getXmlInt("numberOfDrops");
  drops.reserve(n);
  Vector2f pos = girlPlayer->getPosition();
  int w = girlPlayer->getScaledWidth();
  int h = girlPlayer->getScaledHeight();
  for (int i = 0; i < n; ++i) {
    drops.push_back( new SmartSprite("Drop", pos, w, h) );
    girlPlayer->attach( drops[i] );
  }
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new PerPixelCollisionStrategy );

  Viewport::getInstance().setfps(0);
  Viewport::getInstance().setObjectToTrack(girlPlayer);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  world.draw();
  cloud.draw();
  tower.draw();
  tree.draw();
  ground.draw();
  

  for(size_t i = 0; i < chicken.size(); ++i){
     chicken.at(i)->draw();
  }
  for(size_t j = 0; j < girl.size(); ++j){
     girl.at(j)->draw();
  }
  for ( const Drawable* drop : drops ) {
     drop->draw();
  }
  strategies[currentStrategy]->draw();
  girlPlayer->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = drops.begin();
  while ( it != drops.end() ) {
    if ( strategies[currentStrategy]->execute(*girlPlayer, **it) ) {
      SmartSprite* doa = *it;
      girlPlayer->detach(doa);
      delete doa;
      it = drops.erase(it);
    }
    else ++it;
  }
}

void Engine::update(Uint32 ticks) {
  for(size_t i = 0; i < chicken.size(); ++i){
     chicken.at(i)->update(ticks);
  }
  for(size_t j = 0; j < girl.size(); ++j){
     girl.at(j)->update(ticks);
  }
  checkForCollisions();
  girlPlayer->update(ticks);
  for ( Drawable* drop : drops ) {
    drop->update( ticks );
  }

  world.update();
  cloud.update();
  tower.update();
  tree.update();
  ground.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(girlPlayer);
  }
  else {
    Viewport::getInstance().setObjectToTrack(chicken[0]);
  }
}

void Engine::setFps(const int fps){
  Viewport::getInstance().setfps(fps);
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(girlPlayer)->left();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(girlPlayer)->right();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(girlPlayer)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(girlPlayer)->down();
      }
      setFps(clock.getFps());
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }

  }
}
