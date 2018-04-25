#include <vector>
#include <utility>
#include <list>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "hud.h"
#include "viewport.h"
#include <utility>

class CollisionStrategy;
//class SmartSprite;
class SubjectSprite;
class MultiSprite;
class Player;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();
  void switchSprite();
  void setFps(const int fps);
  //explicitly disallow compiler generated functions
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  
private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  World world;
  World cloud;
  World tower;
  World tree;
  World ground;

  Hud hud;
  Viewport& viewport;

  //polymorphic vector for coherent animation
  std::vector<Drawable*> horses;
  std::vector<Drawable*> cows;
  std::vector<Drawable*> sheeps;
  std::vector<Drawable*> pigs;
  std::vector<Drawable*> chickens;

  std::vector<MultiSprite*> birds;
  World set;
  Player* girlPlayer;
  std::list<std::string> catchedList;
  std::list<std::string> catchingList;
  std::list<std::pair<std::string, Vector2f> > releaseList; 
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  int currentSprite;
  bool collision;

  bool makeVideo;
  bool showHud;

  void draw() const;
  void update(Uint32);

  void printScales() const;
};
