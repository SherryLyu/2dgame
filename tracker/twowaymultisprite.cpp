#include "twowaymultisprite.h"
#include "gamedata.h"
#include "renderContext.h"

Vector2f TwowaymultiSprite::makeLocation(int sx, int sy) const {
  int width = Gamedata::getInstance().getXmlInt("world/width");
  float newsx = Gamedata::getInstance().getRandFloat(sx/2,width-sx);
  return Vector2f(newsx, sy);
}

Vector2f TwowaymultiSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-5,vx+50);
  float newvy = Gamedata::getInstance().getRandFloat(vy,vy);

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
           makeLocation(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  catcherId(""),
  identity(id),
  visible(true),
  releasing(false),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  originalY(Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
  originalVX(Gamedata::getInstance().getXmlInt(name+"/speedX"))
{ }

TwowaymultiSprite::TwowaymultiSprite(const TwowaymultiSprite& s) :
  Drawable(s), 
  images( s.images ),
  catcherId( s.catcherId ),
  identity( s.identity ),
  visible( s.visible ),
  releasing( s.releasing ),
  currentFrame( s.currentFrame ),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  originalY( s.originalY ),
  originalVX( s.originalVX )
  { }

TwowaymultiSprite& TwowaymultiSprite::operator=(const TwowaymultiSprite& s) {
  Drawable::operator=(s);
  images = ( s.images );
  catcherId = ( s.catcherId );
  identity = ( s.identity );
  visible = ( s.visible );
  releasing = ( s.releasing );
  currentFrame = ( s.currentFrame );
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  originalY = ( s.originalY );
  originalVX = ( s.originalVX );
  return *this;
}

void TwowaymultiSprite::draw() const { 
  if(visible || releasing){
    images[currentFrame]->draw(getX(), getY(), getScale());
  }
}

void TwowaymultiSprite::update(Uint32 ticks) { 
  //create animation 
  if(releasing){
    if(getY() < originalY){
      setVelocity(Vector2f(0, 50));
    }else{
      releasing = false;
      if (getName().find("Reverse") != std::string::npos) {
        setVelocity(Vector2f(-originalVX, 0));
      }else{
        setVelocity(Vector2f(originalVX, 0));
      } 
    }
  }
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
