#include "player.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "explodingSprite.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::~Player( ) { if (explosion) delete explosion; }

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  explosion(nullptr),
  jumpingStatus(false),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  currentname(name),
  lastMode(STAND),
  currentMode(STAND),
  initialVelocity(getVelocity())
{ }

Player::Player(const Player& s) :
  Drawable(s), 
  images(s.images),
  explosion(s.explosion),
  jumpingStatus(s.getJumpStatus()),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  currentname(s.getName()),
  lastMode(s.lastMode),
  currentMode(s.currentMode),
  initialVelocity( s.initialVelocity )
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = (s.images);
  explosion = s.explosion;
  jumpingStatus = s.getJumpStatus();
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  currentname = ( s.getName() );
  lastMode = (s.lastMode);
  currentMode = ( s.currentMode );
  initialVelocity = ( s.initialVelocity );
  return *this;
}

void Player::explode() {
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

void Player::draw() const { 
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void Player::right() { 
  if ( getX() < worldWidth-getScaledWidth()) {
    //std::cout << "Here comes turing right (1): " << currentMode << ". With image: " << getName() << std::endl;
    //reverse the direction of sprite
    currentname = "Girl";
    images = (ImageFactory::getInstance().getImages (currentname));
    setVelocityX(initialVelocity[0]);
    lastMode = currentMode;
    currentMode = WALKRIGHT;
    //std::cout << "Here comes turing right (2): " << currentMode << ". With image: " << currentname << std::endl;
  }
} 

void Player::left()  { 
  if ( getX() > 0) {
    //reverse the direction of sprite
    currentname = "GirlReverse";
    images = (ImageFactory::getInstance().getImages (currentname));
    setVelocityX(-initialVelocity[0]);
    lastMode = currentMode;
    currentMode = WALKLEFT;
  }
} 

void Player::stand()  {
  if (currentname.find("Reverse") != std::string::npos) {
    currentname = "GirlStandReverse";
  }else{
    currentname = "GirlStand";
  }
  images = (ImageFactory::getInstance().getImages (currentname));
  lastMode = currentMode;
  currentMode = STAND;
}

void Player::down()  {
  if ( getY() < 345) {//check if in jump condition
    if (currentname.find("Reverse") != std::string::npos) {
      currentname = "GirlDownReverse";
    }else{
      currentname = "GirlDown";
    }
    images = (ImageFactory::getInstance().getImages (currentname));
    setVelocityY( initialVelocity[1] );
    lastMode = currentMode;
    currentMode = DOWN;
  }
}

void Player::jump(){
  
  if ( getY() > 0) {
    if( getY() > 250){
      if (currentname.find("Reverse") != std::string::npos) {
        currentname = "GirlUpReverse";
      }else{
        currentname = "GirlUp";
      }
      images = (ImageFactory::getInstance().getImages (currentname));
      setVelocityY( -initialVelocity[1] );
      lastMode = currentMode;
      currentMode = JUMP;
      jumpingStatus = true;
    }
  }
}

void Player::hit() { 
  if (currentname.find("Reverse") != std::string::npos) {
    currentname = "GirlHitReverse";
  }else{
    currentname = "GirlHit";
  }
  images = (ImageFactory::getInstance().getImages (currentname));
  lastMode = currentMode;
  currentMode = HIT;
}

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
}

void Player::update(Uint32 ticks) { 
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  
  if(currentMode == HIT){
    currentMode = lastMode;
  }

  if(currentMode == STAND){
    stand();
  }
  if(currentMode == WALKRIGHT){
    //std::cout << "Here comes change mode: " << currentMode << ". With image: " << getName() << std::endl;
    right();
    currentMode = STAND;
  }
  if(currentMode == WALKLEFT){
    left();
    currentMode = STAND;
  }
  if(currentMode == JUMP){
    jump();
    if(currentFrame == 6){
      currentMode = DOWN;
      currentFrame = 0;
    }
  }
  if(currentMode == DOWN){
    down();
    if(currentFrame == 4){
      currentMode = STAND;
      currentFrame = 0;
      jumpingStatus = false;
    }
  }
  advanceFrame(ticks);
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  /*if ( getY() < 0) {
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
  } */

  stop();
}
