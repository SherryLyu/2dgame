#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "hud.h"
#include "viewport.h"

class CollisionStrategy;
class SmartSprite;
class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  void play();
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
  std::vector<Drawable*> rabbits;
  std::vector<Drawable*> chickens;
  World set;
  std::vector<SmartSprite*> drops;
  SubjectSprite* girlPlayer;
  std::vector<CollisionStrategy*> strategies;
  int currentStrategy;
  int currentSprite;
  bool collision;

  bool makeVideo;
  bool showHud;
  bool jump;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
