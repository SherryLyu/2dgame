#include "player.h"
#include "gamedata.h"
//#include "imageFactory.h"
#include "renderContext.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentname(name),
  initialVelocity(getVelocity())
{ }

Player::Player(const Player& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentname(s.getName()),
  initialVelocity( s.initialVelocity )
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = (s.images);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  initialVelocity = ( s.initialVelocity );
  return *this;
}

void Player::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void Player::jump(){
  if (currentname.find("Reverse") != std::string::npos) {
    images = (RenderContext::getInstance()->getImages ("GirlUpReverse"));
  }else{
    images = (RenderContext::getInstance()->getImages ("GirlUp"));
  }
  
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1]);
  }
}

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    //reverse the direction of sprite
    std::string temp = currentname.substr(0, currentname.find("Reverse"));
    images = (RenderContext::getInstance()->getImages(temp));
    setVelocityX(initialVelocity[0]);
  }
} 
void Player::left()  { 
  if ( getX() > 0) {
    //reverse the direction of sprite
    if (currentname.find("Reverse") == std::string::npos) {
      currentname += "Reverse";
    }
    images = (RenderContext::getInstance()->getImages (currentname));
    
    setVelocityX(-initialVelocity[0]);
  }
} 
void Player::up()    { 
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down()  {
  if ( getY() < worldHeight-getScaledHeight() && getY() < 200) {
    setVelocityY( initialVelocity[1]);
    if (currentname.find("Reverse") != std::string::npos) {
      images = (RenderContext::getInstance()->getImages ("GirlDownReverse"));
    }else{
      images = (RenderContext::getInstance()->getImages ("GirlDown"));
    }
  }
}

void Player::update(Uint32 ticks) { 
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
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  } 

  stop();
}
