#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "twowaymultisprite.h"
#include "multisprite.h"
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
  horses(),
  cows(),
  sheeps(),
  pigs(),
  chickens(),
  birds(),  
  set("set", Gamedata::getInstance().getXmlInt("set/factor")),
  girlPlayer(new Player("Girl")),
  catchedList(),
  catchingList(),
  releaseList(),
  strategies(),
  currentStrategy(0),
  currentSprite(0),
  collision(false),
  makeVideo( false ),
  showHud( false )
{
  int nofhorses = Gamedata::getInstance().getXmlInt("numberOfHorses");
  int nofcows = Gamedata::getInstance().getXmlInt("numberOfCows");
  int nofsheeps = Gamedata::getInstance().getXmlInt("numberOfSheeps");
  int nofpigs = Gamedata::getInstance().getXmlInt("numberOfPigs");
  int nofchickens = Gamedata::getInstance().getXmlInt("numberOfChickens");
  int nofbirds = Gamedata::getInstance().getXmlInt("numberOfBirds");

  birds.reserve(nofbirds);
  horses.reserve(nofhorses);
  cows.reserve(nofcows);
  sheeps.reserve(nofsheeps);
  pigs.reserve(nofpigs);
  chickens.reserve(nofchickens);
  

  for (int i = 0; i < nofhorses; ++i) {
    horses.push_back( new TwowaymultiSprite("Horse", "Horse"+std::to_string(i)) );
  }
  for (int i = 0; i < nofcows; ++i) {
    cows.push_back( new TwowaymultiSprite("Cow", "Cow"+std::to_string(i)) );
  }
  for (int i = 0; i < nofsheeps; ++i) {
    sheeps.push_back( new TwowaymultiSprite("Sheep", "Sheep"+std::to_string(i)) );
  }
  for (int i = 0; i < nofpigs; ++i) {
    pigs.push_back( new TwowaymultiSprite("Pig", "Pig"+std::to_string(i)) );
  }
  for (int i = 0; i < nofchickens; ++i) {
    chickens.push_back( new TwowaymultiSprite("Chicken", "Chicken"+std::to_string(i)) );
  }
  for (int i = 0; i < nofbirds; ++i) {
    birds.push_back( new MultiSprite("Bird", "Bird"+std::to_string(i)) );
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

  if(chickens.empty() && cows.empty() && horses.empty() 
    && sheeps.empty() && pigs.empty()){
    girlPlayer->fall();
  }else{
    auto it = birds.begin();
    while ( it != birds.end() ) {
      //collision detection for player hit bird(enemy)
      if ( strategies[currentStrategy]->execute(*girlPlayer, **it) ) { 
        if ( static_cast<Player*>(girlPlayer)->getHitStatus() ) {
          static_cast<MultiSprite*>(*it)->releaseAnimal(0);
          (*it)->explode();
        }
      }
      (*it)->draw();
      ++it;
    }
    
    for ( Drawable* pig : pigs ) {
      pig->draw();
    }
    for ( Drawable* sheep : sheeps ) {
      sheep->draw();
    }
    for ( Drawable* horse : horses ) {
      horse->draw();
    }
    for ( Drawable* cow : cows ) {
      cow->draw();
    }
    for ( Drawable* chicken : chickens ) {
      chicken->draw();
    }
  }

  set.draw();
  if(chickens.empty() && cows.empty() && horses.empty() 
    && sheeps.empty() && pigs.empty()){
    hud.losing(renderer);
    girlPlayer->draw();
    clock.pause();
  }else if(birds.empty()){
    hud.winning(renderer);
    girlPlayer->draw();
    clock.pause();
  }else{
    girlPlayer->draw();
  }

  //check if show help hud
  if(showHud){
    hud.help(renderer);
    strategies[currentStrategy]->draw();
  }
  int currentCount = chickens.size() + cows.size() + horses.size() + sheeps.size() + pigs.size();
  hud.count(birds.size(), currentCount);

  viewport.draw();
  SDL_RenderPresent(renderer);
}


void Engine::update(Uint32 ticks) {
  girlPlayer->update(ticks);
  set.update();

  auto it = birds.begin();
  while ( it != birds.end() ) {
    (*it)->update( ticks );
    //check if bird(enemy) finished explosion
    if ( (*it)->hasExploded() ) {
      if( static_cast<MultiSprite*>(*it)->getCatchedId() != "" ) {
        std::string catchedid = static_cast<MultiSprite*>(*it)->getCatchedId();
        releaseList.push_back(catchedid);
        catchingList.remove(catchedid);
        static_cast<MultiSprite*>(*it)->releaseAnimal(1);
      }
      delete *it;
      it = birds.erase(it);
    }
    else {
      std::string catcherid = static_cast<MultiSprite*>(*it)->getIdentity();
      auto itforhorses = horses.begin();
      while ( itforhorses != horses.end() ){
        //keep catch and be catched relationship clear, 
        //in order to release when player kill the enemy
        //or sadly check the animal has been catched away
        std::string animalId = static_cast<TwowaymultiSprite*>(*itforhorses)->getIdentity();
        if ( static_cast<TwowaymultiSprite*>(*itforhorses)->getCatcherId() == catcherid
          && static_cast<MultiSprite*>(*it)->getCatchedId() != animalId ) {
          catchedList.push_back(animalId);
          catchingList.remove(animalId);
        }else if ( strategies[currentStrategy]->execute(**it, **itforhorses) 
          && static_cast<MultiSprite*>(*it)->getCatchedId() == "") { 
          //collision detection for catching animals
          std::string animalName = (*itforhorses)->getName();
          static_cast<MultiSprite*>(*it)->catchAnimal(animalName, animalId);
          catchingList.push_back(animalId);
          
          static_cast<TwowaymultiSprite*>(*itforhorses)->setCatcherId(catcherid);
        }
        ++itforhorses;
      }
      auto itforcows = cows.begin();
      while ( itforcows != cows.end() ){
        std::string animalId = static_cast<TwowaymultiSprite*>(*itforcows)->getIdentity();
        if ( static_cast<TwowaymultiSprite*>(*itforcows)->getCatcherId() == catcherid
          && static_cast<MultiSprite*>(*it)->getCatchedId() != animalId ) {
          catchedList.push_back(animalId);
          catchingList.remove(animalId);
        }else if ( strategies[currentStrategy]->execute(**it, **itforcows) 
          && static_cast<MultiSprite*>(*it)->getCatchedId() == "") { 
          std::string animalName = (*itforcows)->getName();
          std::string animalId = static_cast<TwowaymultiSprite*>(*itforcows)->getIdentity();
          static_cast<MultiSprite*>(*it)->catchAnimal(animalName, animalId);
          catchingList.push_back(animalId);

          static_cast<TwowaymultiSprite*>(*itforcows)->setCatcherId(catcherid);
        }
        ++itforcows;
      }
      auto itforsheeps = sheeps.begin();
      while ( itforsheeps != sheeps.end() ){
        std::string animalId = static_cast<TwowaymultiSprite*>(*itforsheeps)->getIdentity();
        if ( static_cast<TwowaymultiSprite*>(*itforsheeps)->getCatcherId() == catcherid
          && static_cast<MultiSprite*>(*it)->getCatchedId() != animalId ) {
          catchedList.push_back(animalId);
          catchingList.remove(animalId);
        }else if ( strategies[currentStrategy]->execute(**it, **itforsheeps) 
          && static_cast<MultiSprite*>(*it)->getCatchedId() == "") { 
          std::string animalName = (*itforsheeps)->getName();
          std::string animalId = static_cast<TwowaymultiSprite*>(*itforsheeps)->getIdentity();
          static_cast<MultiSprite*>(*it)->catchAnimal(animalName, animalId);
          catchingList.push_back(animalId);

          static_cast<TwowaymultiSprite*>(*itforsheeps)->setCatcherId(catcherid);
        }
        ++itforsheeps;
      }
      auto itforpigs = pigs.begin();
      while ( itforpigs != pigs.end() ){
        std::string animalId = static_cast<TwowaymultiSprite*>(*itforpigs)->getIdentity();
        if ( static_cast<TwowaymultiSprite*>(*itforpigs)->getCatcherId() == catcherid
          && static_cast<MultiSprite*>(*it)->getCatchedId() != animalId ) {
          catchedList.push_back(animalId);
          catchingList.remove(animalId);
        }else if ( strategies[currentStrategy]->execute(**it, **itforpigs) 
          && static_cast<MultiSprite*>(*it)->getCatchedId() == "") { 
          std::string animalName = (*itforpigs)->getName();
          std::string animalId = static_cast<TwowaymultiSprite*>(*itforpigs)->getIdentity();
          static_cast<MultiSprite*>(*it)->catchAnimal(animalName, animalId);
          catchingList.push_back(animalId);

          static_cast<TwowaymultiSprite*>(*itforpigs)->setCatcherId(catcherid);
        }
        ++itforpigs;
      }
      auto itforchickens = chickens.begin();
      while ( itforchickens != chickens.end() ){
        std::string animalId = static_cast<TwowaymultiSprite*>(*itforchickens)->getIdentity();
        if ( static_cast<TwowaymultiSprite*>(*itforchickens)->getCatcherId() == catcherid
          && static_cast<MultiSprite*>(*it)->getCatchedId() != animalId ) {
          catchedList.push_back(animalId);
          catchingList.remove(animalId);
        }else if ( strategies[currentStrategy]->execute(**it, **itforchickens) 
          && static_cast<MultiSprite*>(*it)->getCatchedId() == "") { 
          std::string animalName = (*itforchickens)->getName();
          std::string animalId = static_cast<TwowaymultiSprite*>(*itforchickens)->getIdentity();
          static_cast<MultiSprite*>(*it)->catchAnimal(animalName, animalId);
          catchingList.push_back(animalId);

          static_cast<TwowaymultiSprite*>(*itforchickens)->setCatcherId(catcherid);
        }
        ++itforchickens;
      }
    ++it;
    }
  }

  //check if animals have been catched
  auto itforchickens = chickens.begin();
  while ( itforchickens != chickens.end() ) {
    (*itforchickens)->update(ticks);
    std::string id = static_cast<TwowaymultiSprite*>(*itforchickens)->getIdentity();
    bool foundCatched = (std::find(catchedList.begin(), catchedList.end(), id) != catchedList.end());
    bool foundCatching = (std::find(catchingList.begin(), catchingList.end(), id) != catchingList.end());
    //check if animals have been catched away or still have chance to be saved
    if(foundCatched || foundCatching){
      if(foundCatched) catchedList.remove(id);
      delete *itforchickens;
      itforchickens = chickens.erase(itforchickens);
    }
    else ++itforchickens;
  }

  auto itforcows = cows.begin();
  while ( itforcows != cows.end() ) {
    (*itforcows)->update(ticks);
    std::string id = static_cast<TwowaymultiSprite*>(*itforcows)->getIdentity();
    bool foundCatched = (std::find(catchedList.begin(), catchedList.end(), id) != catchedList.end());
    bool foundCatching = (std::find(catchingList.begin(), catchingList.end(), id) != catchingList.end());
    if(foundCatched || foundCatching){
      if(foundCatched) catchedList.remove(id);
      delete *itforcows;
      itforcows = cows.erase(itforcows);
    }
    else ++itforcows;
  }

  auto itforhorses = horses.begin();
  while ( itforhorses != horses.end() ) {
    (*itforhorses)->update(ticks);
    std::string id = static_cast<TwowaymultiSprite*>(*itforhorses)->getIdentity();
    bool foundCatched = (std::find(catchedList.begin(), catchedList.end(), id) != catchedList.end());
    bool foundCatching = (std::find(catchingList.begin(), catchingList.end(), id) != catchingList.end());
    if(foundCatched || foundCatching){
      if(foundCatched) catchedList.remove(id);
      delete *itforhorses;
      itforhorses = horses.erase(itforhorses);
    }
    else ++itforhorses;
  }

  auto itforsheeps = sheeps.begin();
  while ( itforsheeps != sheeps.end() ) {
    (*itforsheeps)->update(ticks);
    std::string id = static_cast<TwowaymultiSprite*>(*itforsheeps)->getIdentity();
    bool foundCatched = (std::find(catchedList.begin(), catchedList.end(), id) != catchedList.end());
    bool foundCatching = (std::find(catchingList.begin(), catchingList.end(), id) != catchingList.end());
    if(foundCatched || foundCatching){
      if(foundCatched) catchedList.remove(id);
      delete *itforsheeps;
      itforsheeps = sheeps.erase(itforsheeps);
    }
    else ++itforsheeps;
  }

  auto itforpigs = pigs.begin();
  while ( itforpigs != pigs.end() ) {
    (*itforpigs)->update(ticks);
    std::string id = static_cast<TwowaymultiSprite*>(*itforpigs)->getIdentity();
    bool foundCatched = (std::find(catchedList.begin(), catchedList.end(), id) != catchedList.end());
    bool foundCatching = (std::find(catchingList.begin(), catchingList.end(), id) != catchingList.end());
    if(foundCatched || foundCatching){
      if(foundCatched) catchedList.remove(id);
      delete *itforpigs;
      itforpigs = pigs.erase(itforpigs);
    }
    else ++itforpigs;
  }
  
  //return animals who has set free by player
  while(!releaseList.empty()){
    std::string f = releaseList.front();
    if (f.compare(0,3,"Cow") == 0){
      cows.push_back( new TwowaymultiSprite("Cow", f) );
    }else if (f.compare(0,3,"Pig") == 0){
      pigs.push_back( new TwowaymultiSprite("Pig", f) );
    }else if (f.compare(0,5,"Horse") == 0){
      horses.push_back( new TwowaymultiSprite("Horse", f) );
    }else if (f.compare(0,5,"Sheep") == 0){
      sheeps.push_back( new TwowaymultiSprite("Sheep", f) );
    }else{
      chickens.push_back( new TwowaymultiSprite("Chicken", f) );
    }   
    releaseList.pop_front();
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

bool Engine::play() {
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
        //restart the game
        if ( keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if ( keystate[SDL_SCANCODE_M] ) {
          currentStrategy = (1 + currentStrategy) % strategies.size();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        //show help hud
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
      if( !static_cast<Player*>(girlPlayer)->getJumpStatus()){
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
  return false;
}
