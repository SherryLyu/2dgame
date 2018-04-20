#include "twowaymultisprite.h"
#include "gamedata.h"
#include "renderContext.h"

Vector2f TwowaymultiSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-5,vx+10);;
  float newvy = Gamedata::getInstance().getRandFloat(vy,vy);;

  return Vector2f(newvx, newvy);
}

void TwowaymultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwowaymultiSprite::TwowaymultiSprite( const std::string& name, const std::string& id ) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  catcherId(""),
  identity(id),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwowaymultiSprite::TwowaymultiSprite(const TwowaymultiSprite& s) :
  Drawable(s), 
  images( s.images ),
  catcherId( s.catcherId ),
  identity( s.identity ),
  currentFrame( s.currentFrame ),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

TwowaymultiSprite& TwowaymultiSprite::operator=(const TwowaymultiSprite& s) {
  Drawable::operator=(s);
  images = ( s.images );
  catcherId = ( s.catcherId );
  identity = ( s.identity );
  currentFrame = ( s.currentFrame );
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void TwowaymultiSprite::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwowaymultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    //when hit the left edge of world, reverse the direction of sprite
    if (getName().find("Reverse") != std::string::npos) {
      setName(getName().substr(0, getName().find("Reverse")));
    }
    images = (RenderContext::getInstance()->getImages(getName()));
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    //when hit the right edge of world, reverse the direction of sprite
    if (getName().find("Reverse") == std::string::npos) {
      setName(getName() + "Reverse");
    }
    images = (RenderContext::getInstance()->getImages (getName()));
    setVelocityX( -fabs( getVelocityX() ) );
  } 


}
