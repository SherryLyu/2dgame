#include <sstream>
#include "viewport.h"
#include "ioMod.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() : 
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")), 
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  username(gdata.getXmlStr("username")), 
  fullname(gdata.getXmlStr("fullname")), 
  objectToTrack(NULL),
  currentfps(0)  
{}

void Viewport::setObjectToTrack(const Drawable *obj) { 
  objectToTrack = obj; 
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::setfps(const int fps) { 
  currentfps = fps; 
}

void Viewport::draw() const {

  std::ostringstream ss;
  ss << currentfps;

  IoMod::getInstance().
    writeText("Current fps: "+ss.str(), 30, 30);
  IoMod::getInstance().
    writeText("Creator name: "+fullname, 30, 400);
  IoMod::getInstance().
    writeText("Username: "+username, 30, 430);
  IoMod::getInstance().
    writeText("Hit F1 for open/close HUD", 615, 30);
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}
