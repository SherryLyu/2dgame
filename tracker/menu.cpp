#include <sstream>
#include "menu.h"
#include "hud.h"

Menu::Menu(SDL_Renderer* rend) :  
  init(TTF_Init()),
  renderer(rend), 
  gdata(Gamedata::getInstance()), 
  hudFrame( {gdata.getXmlInt("Menu/startLocX"),
             gdata.getXmlInt("Menu/startLocY"),
             gdata.getXmlInt("Menu/width"),
             gdata.getXmlInt("Menu/height")}
  ),
  customizeFrame( {gdata.getXmlInt("Customize/startLocX"),
             gdata.getXmlInt("Customize/startLocY"),
             gdata.getXmlInt("Customize/width"),
             gdata.getXmlInt("Customize/height")}
  ),
  girlMenu( {gdata.getXmlInt("GirlMenu/startLocX"),
             gdata.getXmlInt("GirlMenu/startLocY"),
             gdata.getXmlInt("GirlMenu/width"),
             gdata.getXmlInt("GirlMenu/height")}
  ),
  fontColor({static_cast<Uint8>(gdata.getXmlInt("customfont/red")),
             static_cast<Uint8>(gdata.getXmlInt("customfont/green")),
             static_cast<Uint8>(gdata.getXmlInt("customfont/blue")),
             static_cast<Uint8>(gdata.getXmlInt("customfont/alpha"))}
  ),
  fontSet(TTF_OpenFont(gdata.getXmlStr("customfont/file").c_str(),
            gdata.getXmlInt("customfont/size"))),
  inputColor({static_cast<Uint8>(gdata.getXmlInt("inputfont/red")),
             static_cast<Uint8>(gdata.getXmlInt("inputfont/green")),
             static_cast<Uint8>(gdata.getXmlInt("inputfont/blue")),
             static_cast<Uint8>(gdata.getXmlInt("inputfont/alpha"))}
  ),
  inputfont(TTF_OpenFont(gdata.getXmlStr("inputfont/file").c_str(),
            gdata.getXmlInt("inputfont/size"))),
  clock( Clock::getInstance() ),
  io( IoMod::getInstance() ),
  options(),
  cusOptions(),
  optionLoc( { gdata.getXmlInt("Menu/optionLoc/x"),
               gdata.getXmlInt("Menu/optionLoc/y")}
           ),
  cusLoc( { gdata.getXmlInt("Customize/optionLoc/x"),
               gdata.getXmlInt("Customize/optionLoc/y")}
           ),
  clicks( {Sprite("clickOff"), Sprite("clickOn")} ),
  underline( Sprite("Space") ),
  currentClick(0),
  currentOption(0),
  spaces(gdata.getXmlInt("Menu/spaces")),
  startClickX(optionLoc[0]-spaces),
  startClickY(optionLoc[1]+spaces),
  clickX(startClickX),
  clickY(startClickY),
  cusCurrentClick(0),
  cusCurrentOption(0),
  cusSpaces(gdata.getXmlInt("Customize/spaces")),
  cusStartClickX(cusLoc[0]),
  cusStartClickY(cusLoc[1]),
  cusClickX(cusStartClickX),
  cusClickY(cusStartClickY),
  chickendata("xmlSpec/chicken.xml"),
  sheepdata("xmlSpec/sheep.xml"),
  pigdata("xmlSpec/pig.xml"),
  horsedata("xmlSpec/horse.xml"),
  cowdata("xmlSpec/cow.xml"),
  birddata("xmlSpec/bird.xml"),
  showControl(false)
{
  if ( init == -1 ) {
    throw std::string("error: Couldn't init font");
  }
  if (fontSet == NULL) {
    throw std::string("error: font not found");
  }
  if (inputfont == NULL) {
    throw std::string("error: font not found");
  }
  int noOfOptions = gdata.getXmlInt("Menu/noOfOptions");
  std::stringstream strm;
  for (int i = 0; i < noOfOptions; ++i) {
    strm.clear();
    strm.str("");
    strm << i;
    std::string option("Menu/option"+strm.str());
    options.push_back(gdata.getXmlStr(option));
  }
  int noOfCusOptions = gdata.getXmlInt("Customize/noOfOptions");
  std::stringstream strm1;
  for (int i = 0; i < noOfCusOptions; ++i) {
    strm1.clear();
    strm1.str("");
    strm1 << i;
    std::string cusOption("Customize/option"+strm1.str());
    cusOptions.push_back(gdata.getXmlStr(cusOption));
  }
}

Menu::~Menu() { 
  TTF_CloseFont(inputfont);
  TTF_CloseFont(fontSet);
  TTF_Quit(); 
}


void Menu::incrIcon() { 
  clickY += spaces; 
  if ( clickY > static_cast<int>(options.size())*spaces+optionLoc[1]) {
    clickY = startClickY;
    currentOption = 0;
  }
  else ++currentOption;
}

void Menu::decrIcon() { 
  clickY -= spaces; 
  if ( clickY < spaces+optionLoc[1]) {
    clickY = startClickY+2*spaces;
    currentOption = options.size()-1;
  }
  else --currentOption;
}
//different way to increase icon in customize page
void Menu::incrCusIcon() { 
  cusClickX += cusSpaces; 
  if ( cusClickX >= static_cast<int>(cusOptions.size())/2*cusSpaces+cusLoc[0] ) {
    cusClickX = cusStartClickX;
    if( cusCurrentOption == static_cast<int>(cusOptions.size())-1 ){
      cusClickY = cusStartClickY;
      cusCurrentOption = 0;
    }else{
      cusClickY += cusSpaces/1.3;
      ++cusCurrentOption;
    }
  }
  else ++cusCurrentOption;
}


//different way to decrease icon in customize page
void Menu::decrCusIcon() { 
  cusClickX -= cusSpaces; 
  if ( cusClickX < cusStartClickX ) {
    cusClickX = cusStartClickX+2*cusSpaces;
    if( cusCurrentOption == static_cast<int>(cusOptions.size())/2 ){
      cusClickY -= cusSpaces/1.3;
      --cusCurrentOption;
    }else{
      cusClickY += cusSpaces/1.3;
      cusCurrentOption = cusOptions.size()-1;
    }
  }
  else --cusCurrentOption;
}

//change the animal number in xml
void Menu::setOptionNum(std::pair<int, int> num) {
  if(num.second == 0) {
    chickendata.setNumber(num.first, "Chickens");
    chickendata.writeTheXml();
  }else if(num.second == 1) {  
    sheepdata.setNumber(num.first, "Sheeps");
    sheepdata.writeTheXml();
  }else if(num.second == 2) {   
    pigdata.setNumber(num.first, "Pigs");
    pigdata.writeTheXml();
  }else if(num.second == 3) { 
    horsedata.setNumber(num.first, "Horses"); 
    horsedata.writeTheXml();
  }else if(num.second == 4) {
    cowdata.setNumber(num.first, "Cows");
    cowdata.writeTheXml();
  }else if(num.second == 5) {
    birddata.setNumber(num.first, "Birds");
    birddata.writeTheXml();
  }
}

void Menu::drawCustomize() const {

  SDL_Texture* bg = IMG_LoadTexture(renderer, gdata.getXmlStr("Customize/file").c_str());
  SDL_Rect output = { customizeFrame.x, customizeFrame.y, customizeFrame.w, customizeFrame.h };
  SDL_RenderCopy(renderer, bg, NULL, &output);

  //current animals numbers
  io.writeText(std::to_string(chickendata.getNumber("Chickens")), customizeFrame.x+200, customizeFrame.y+180);
  io.writeText(std::to_string(sheepdata.getNumber("Sheeps")), customizeFrame.x+400, customizeFrame.y+180);
  io.writeText(std::to_string(pigdata.getNumber("Pigs")), customizeFrame.x+600, customizeFrame.y+180);
  io.writeText(std::to_string(horsedata.getNumber("Horses")), customizeFrame.x+200, customizeFrame.y+330);
  io.writeText(std::to_string(cowdata.getNumber("Cows")), customizeFrame.x+400, customizeFrame.y+330);
  io.writeText(std::to_string(birddata.getNumber("Birds")), customizeFrame.x+600, customizeFrame.y+330);

  SDL_DestroyTexture(bg);
  underline.getImage()->draw(0, 0, cusClickX, cusClickY);
}

//get user input of animal numbers
void Menu::getInputEventLoop() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  drawCustomize();
  std::string inNumber = " ";
  std::string msg1 = "Change one at a time! Restart your game by r when you back!";
  std::string msg2 = "Press return/esc when finished.";

  while ( !done ) {
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || 
            keystate[SDL_SCANCODE_Q] ||
            keystate[SDL_SCANCODE_RETURN] ) {
          done = true;
          break;
        }
        if (keystate[SDL_SCANCODE_D] ) incrCusIcon();
        if (keystate[SDL_SCANCODE_A] ) decrCusIcon();
        if (keystate[SDL_SCANCODE_BACKSPACE]){
          if(inNumber.length() > 1) inNumber = inNumber.substr(0, inNumber.length() - 1);
        }
        if ( keystate[SDL_SCANCODE_0] ) {
          inNumber += '0';
        }
        else if ( keystate[SDL_SCANCODE_1] ) {
          inNumber += '1';
        }
        else if ( keystate[SDL_SCANCODE_2] ) {
          inNumber += '2';
        }
        else if ( keystate[SDL_SCANCODE_3] ) {
          inNumber += '3';
        }
        else if ( keystate[SDL_SCANCODE_4] ) {
          inNumber += '4';
        }
        else if ( keystate[SDL_SCANCODE_5] ) {
          inNumber += '5';
        }
        else if ( keystate[SDL_SCANCODE_6] ) {
          inNumber += '6';
        }
        else if ( keystate[SDL_SCANCODE_7] ) {
          inNumber += '7';
        }
        else if ( keystate[SDL_SCANCODE_8] ) {
          inNumber += '8';
        }
        else if ( keystate[SDL_SCANCODE_9] ) {
          inNumber += '9';
        }
      }
      drawCustomize();
      io.writeText(msg1, customizeFrame.x+130, customizeFrame.y+80);
      io.writeText(inNumber, cusClickX, cusClickY+15, inputfont, inputColor);
      io.writeText(msg2, customizeFrame.x+260, customizeFrame.y+370);
      SDL_RenderPresent(renderer);
    }
  }
  setOptionNum(std::make_pair(atoi( inNumber.c_str() ), cusCurrentOption));
}


void Menu::draw() const {

  SDL_Texture* textureBox = IMG_LoadTexture(renderer, gdata.getXmlStr("Menu/file").c_str());
  SDL_Rect outputBox = { hudFrame.x, hudFrame.y, hudFrame.w, hudFrame.h };
  SDL_RenderCopy(renderer, textureBox, NULL, &outputBox);

  if(showControl){    
    io.writeText("a : player turn left", 180, 100);
    io.writeText("d : player turn right", 180, 140);
    io.writeText("space : jump", 180, 180);
    io.writeText("h : hit enemy", 180, 220);
    io.writeText("r : restart", 180, 260);
    io.writeText("g : turn on/off god mode", 180, 300);
    io.writeText("F1 : turn on/off HUD", 180, 340);

    io.writeText("m : open menu", 440, 100);
    io.writeText("w : move up in menu", 440, 140);
    io.writeText("s : move down in menu", 440, 180);
    io.writeText("a : move left in customize", 440, 220);
    io.writeText("d : move right in customize", 440, 260);
    io.writeText("enter : enter/quit selection", 440, 300);
    io.writeText("esc : quit menu/game", 440, 340);
  }else{
    SDL_Texture* textureGirl = IMG_LoadTexture(renderer, gdata.getXmlStr("GirlMenu/file").c_str());
    SDL_Rect outputGirl = { girlMenu.x, girlMenu.y, girlMenu.w, girlMenu.h };
    SDL_RenderCopy(renderer, textureGirl, NULL, &outputGirl);
    SDL_DestroyTexture(textureGirl);
    int space = spaces;
    for ( const std::string& option : options ) {
      io.writeText(option, optionLoc[0], optionLoc[1]+space, fontSet, fontColor);
      space += spaces;
    }
    
    clicks[currentClick].getImage()->draw(0, 0, clickX, clickY);
  }

  SDL_DestroyTexture(textureBox);
  
}

