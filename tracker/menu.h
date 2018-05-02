#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <string>
#include "gamedata.h"
#include "numberdata.h"
#include "clock.h"
#include "ioMod.h"
#include "sprite.h"

class Menu {
public:
  Menu(SDL_Renderer*);
  ~Menu();
  void draw() const;
  void update();
  void incrIcon();
  void decrIcon();
  void incrCusIcon();
  void decrCusIcon();
  void chooseOn()  { currentClick = 1; }
  void chooseOff() { currentClick = 0; }
  void setOptionNum(std::pair<int, int>);
  void getInputEventLoop();
  int getOptionNo() const { return currentOption; }
  void setShowControl(const bool b) { 
    showControl = b;   
  }
  

  Menu(const Menu&) = delete;
  Menu& operator=(const Menu&) = delete;
private:
  int init;
  SDL_Renderer* renderer;
  Gamedata& gdata;
  SDL_Rect hudFrame;
  SDL_Rect customizeFrame;
  SDL_Rect girlMenu;
  SDL_Color fontColor;
  TTF_Font* fontSet;
  SDL_Color inputColor;
  TTF_Font* inputfont;
  Clock& clock;
  IoMod& io;
  std::vector<std::string> options;
  std::vector<std::string> cusOptions;
  std::vector<int> optionLoc;
  std::vector<int> cusLoc;
  std::vector<Sprite> clicks;
  Sprite underline;
  int currentClick;
  int currentOption;
  int spaces;
  int startClickX;
  int startClickY;
  int clickX;
  int clickY;
  int cusCurrentClick;
  int cusCurrentOption;
  int cusSpaces;
  int cusStartClickX;
  int cusStartClickY;
  int cusClickX;
  int cusClickY;
  NumberData chickendata;
  NumberData sheepdata;
  NumberData pigdata; 
  NumberData horsedata; 
  NumberData cowdata;
  NumberData birddata;
  bool showControl;
  void drawCustomize() const;
};
