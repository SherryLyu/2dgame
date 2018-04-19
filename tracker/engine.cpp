#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "twowaymultisprite.h"
//#include "smartSprite.h"
#include "multisprite.h"
//#include "subjectSprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"

Engine::~Engine() { 
  delete girlPlayer;

  for( Drawable* bird : birds){
    delete bird;
  }
  for( Drawable* chicken : chickens){
    delete chicken;
  }
  for( Drawable* rabbit : rabbits){
    delete rabbit;
  }
  for( Drawable* pig : pigs){
    delete pig;
  }
  for( Drawable* sheep : sheeps){
    delete sheep;
  }
  for( Drawable* horse : horses){
    delete horse;
  }
  for( Drawable* cow : cows){
    delete cow;
  }
  

  for ( CollisionStrategy* strategy : strategies ) {
    delete strategy;
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
  hud(),
  viewport( Viewport::getInstance() ),
  //use initialization list 
  horses({new TwowaymultiSprite("Horse"), new TwowaymultiSprite("Horse")}),
  cows({new TwowaymultiSprite("Cow"), new TwowaymultiSprite("Cow")}),
  sheeps({new TwowaymultiSprite("Sheep"), new TwowaymultiSprite("Sheep")}),
  pigs({new TwowaymultiSprite("Pig"), new TwowaymultiSprite("Pig")}),
  rabbits({new TwowaymultiSprite("Rabbit"), new TwowaymultiSprite("Rabbit")}),
  chickens({new TwowaymultiSprite("Chicken"), new TwowaymultiSprite("Chicken")}),
  //birds({new TwowaymultiSprite("Bird"), new TwowaymultiSprite("Bird"), new TwowaymultiSprite("Bird")}),
  set("set", Gamedata::getInstance().getXmlInt("set/factor")),
  birds(),  
  girlPlayer(new Player("Girl")),
  strategies(),
  currentStrategy(0),
  currentSprite(0),
  collision(false),
  makeVideo( false ),
  showHud( false )
{
  int n = Gamedata::getInstance().getXmlInt("numberOfBirds");
  birds.reserve(n);
  /*Vector2f pos = girlPlayer->getPosition();
  int w = girlPlayer->getScaledWidth();
  int h = girlPlayer->getScaledHeight();
  for (int i = 0; i < n; ++i) {
    birds.push_back( new SmartSprite("Bird", pos, w, h) );
    girlPlayer->attach( birds[i] );;
  }*/
  for (int i = 0; i < n; ++i) {
    birds.push_back( new MultiSprite("Bird") );
  }
  strategies.push_back( new PerPixelCollisionStrategy );
  strategies.push_back( new RectangularCollisionStrategy );
  strategies.push_back( new MidPointCollisionStrategy );
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
  
  //check if show hud
  if(showHud){
    hud.draw(renderer);
    strategies[currentStrategy]->draw();
  }
  for ( const Drawable* pig : pigs ) {
     pig->draw();
  }
  for ( const Drawable* sheep : sheeps ) {
     sheep->draw();
  }
  for ( const Drawable* horse : horses ) {
     horse->draw();
  }
  for ( const Drawable* cow : cows ) {
     cow->draw();
  }
  for ( const Drawable* rabbit : rabbits ) {
     rabbit->draw();
  }
  for ( const Drawable* chicken : chickens ) {
     chicken->draw();
  }

  set.draw();

  auto it = birds.begin();
  while ( it != birds.end() ) {
    //collision detection
    if ( strategies[currentStrategy]->execute(*girlPlayer, **it) ) { 
      if ( static_cast<Player*>(girlPlayer)->getHitStatus() ) {
        (*it)->explode();
      }
    }
    (*it)->draw();
    ++it;
  }
  
  girlPlayer->draw();

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  girlPlayer->update(ticks);

  auto it = birds.begin();
  while ( it != birds.end() ) {
    (*it)->update( ticks );
    if ( (*it)->hasExploded() ) {
      delete *it;
      it = birds.erase(it);
    }
    else ++it;
  }

  set.update();

  for(size_t i = 0; i < chickens.size(); ++i){
     chickens.at(i)->update(ticks);
  }
  for(size_t i = 0; i < rabbits.size(); ++i){
     rabbits.at(i)->update(ticks);
  }
  for(size_t i = 0; i < cows.size(); ++i){
     cows.at(i)->update(ticks);
  }
  for(size_t i = 0; i < horses.size(); ++i){
     horses.at(i)->update(ticks);
  }
  for(size_t i = 0; i < sheeps.size(); ++i){
     sheeps.at(i)->update(ticks);
  }
  for(size_t i = 0; i < pigs.size(); ++i){
     pigs.at(i)->update(ticks);
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
    Viewport::getInstance().setObjectToTrack(chickens[0]);
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
      if(event.type == SDL_KEYDOWN && event.key.repeat == 0) {
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
        if ( keystate[SDL_SCANCODE_F1] && !showHud) {
          showHud = true;
        }
        else if ( keystate[SDL_SCANCODE_F1] && showHud) {
          showHud = false;
        }
        //jump
        if (keystate[SDL_SCANCODE_SPACE]) {
          static_cast<Player*>(girlPlayer)->jump();
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
      if( !static_cast<Player*>(girlPlayer)->getJumpStatus() ){
        if (keystate[SDL_SCANCODE_A]) {
          static_cast<Player*>(girlPlayer)->left();
        }
        if (keystate[SDL_SCANCODE_D]) {
          static_cast<Player*>(girlPlayer)->right();
        }
      }
      //hit 
      if (keystate[SDL_SCANCODE_H]) {
        static_cast<Player*>(girlPlayer)->hit();
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
