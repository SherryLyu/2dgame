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
  hitStatus(false),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  lastMode(STAND),
  currentMode(STAND),
  initialVelocity(getVelocity()),
  gravity(Gamedata::getInstance().getXmlFloat(name+"/gravity"))
{ }

Player::Player(const Player& s) :
  Drawable(s), 
  images(s.images),
  explosion(s.explosion),
  jumpingStatus(s.getJumpStatus()),
  hitStatus(s.getHitStatus()),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  lastMode( s.lastMode ),
  currentMode( s.currentMode ),
  initialVelocity( s.initialVelocity ),
  gravity( s.gravity )
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = ( s.images );
  explosion = ( s.explosion );
  jumpingStatus = ( s.getJumpStatus() );
  hitStatus = ( s.getHitStatus() );
  currentFrame = ( s.currentFrame );
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  lastMode = (s.lastMode);
  currentMode = ( s.currentMode );
  initialVelocity = ( s.initialVelocity );
  gravity = ( s.gravity );
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
    setName("Girl");
    images = (ImageFactory::getInstance().getImages (getName()));
    setVelocityX(initialVelocity[0]);
    lastMode = currentMode;
    currentMode = WALKRIGHT;
  }
} 

void Player::left()  { 
  if ( getX() > 0) {
    //reverse the direction of sprite
    setName("GirlReverse");
    images = (ImageFactory::getInstance().getImages (getName()));
    setVelocityX(-initialVelocity[0]);
    lastMode = currentMode;
    currentMode = WALKLEFT;
  }
} 

void Player::stand()  {
  if (getName().find("Reverse") != std::string::npos) {
    setName("GirlStandReverse");
  }else{
    setName("GirlStand");
  }
  images = (ImageFactory::getInstance().getImages (getName()));
  lastMode = currentMode;
  currentMode = STAND;
}

void Player::down()  {
  if ( getY() < 304) {//check if in jump condition
    if (getName().find("Reverse") != std::string::npos) {
      setName("GirlDownReverse");
    }else{
      setName("GirlDown");
    }
    images = (ImageFactory::getInstance().getImages (getName()));
    setVelocityY( initialVelocity[1] );
    lastMode = currentMode;
    currentMode = DOWN;
  }
}

void Player::jump(){
  if ( getY() > 0) {
    if( getY() > 215){
      if (getName().find("Reverse") != std::string::npos) {
        setName("GirlUpReverse");
      }else{
        setName("GirlUp");
      }
      images = (ImageFactory::getInstance().getImages (getName()));
      setVelocityY( -initialVelocity[1] );
      lastMode = currentMode;
      currentMode = JUMP;
      jumpingStatus = true;
    }
  }
}

void Player::hit() { 
  if (getName().find("Reverse") != std::string::npos) {
    setName("GirlHitReverse");
  }else{
    setName("GirlHit");
  }
  images = (ImageFactory::getInstance().getImages (getName()));
  lastMode = currentMode;
  currentMode = HIT;
  hitStatus = true;
}

void Player::stop() { 
  setVelocity( Vector2f(0, 0) );
}

void Player::fall() {
  if (getName().find("Reverse") != std::string::npos) {
    setName("GirlFallReverse");
  }else{
    setName("GirlFall");
  }
  images = (ImageFactory::getInstance().getImages (getName()));
  currentMode = FALL;
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
    hitStatus = false;
  }

  if(currentMode == STAND){
    stand();
  }
  if(currentMode == WALKRIGHT){
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
    if(currentFrame == 7){
      currentMode = STAND;
      currentFrame = 0;
      jumpingStatus = false;
      setPosition(Vector2f(getPosition()[0], 304));
    }
  }
  advanceFrame(ticks);
  if(currentMode == DOWN){
    float incrY = gravity * static_cast<float>(ticks) * 0.001;
    if(getVelocityY() != 0){
      setVelocityY(getVelocityY() + incrY);
    }
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  stop();
}
