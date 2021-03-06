#include "multisprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include "explodingSprite.h"

Vector2f MultiSprite::makeLocation(int sx, int sy) const {
  int width = Gamedata::getInstance().getXmlInt("world/width");
  float newsx = Gamedata::getInstance().getRandFloat(sx/2,width-sx);
  float newsy = Gamedata::getInstance().getRandFloat(sy-10,sy-300);
  return Vector2f(newsx, newsy);
}

Vector2f MultiSprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-10,vx+10);
  float newvy = Gamedata::getInstance().getRandFloat(vy,vy);
  return Vector2f(newvx, newvy);
}

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::~MultiSprite( ) { if (explosion) delete explosion; }

MultiSprite::MultiSprite( const std::string& name, const std::string& id ) :
  Drawable(name, 
           makeLocation(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( RenderContext::getInstance()->getImages(name) ),
  explosion(nullptr),
  explosionEnd(false),
  initialName(name),
  initialVelocity(getVelocity()),
  initialPosition(getPosition()),
  catchedId(""),
  identity(id),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  images( s.images ),
  explosion( s.explosion ),
  explosionEnd( s.explosionEnd ),
  initialName( s.initialName ),
  initialVelocity( s.initialVelocity ),
  initialPosition( s.initialPosition ),
  catchedId( s.catchedId ),
  identity( s.identity ),
  currentFrame( s.currentFrame ),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = ( s.images );
  explosion = ( s.explosion );
  explosionEnd = ( s.explosionEnd );
  initialName = ( s.initialName );
  initialVelocity = ( s.initialVelocity );
  initialPosition = ( s.initialPosition );
  catchedId = ( s.catchedId );
  identity = ( s.identity );
  currentFrame = ( s.currentFrame );
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

//change to explosion image 
void MultiSprite::explode() {
  if ( !explosion ) {
    Sprite 
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    explosion = new ExplodingSprite(sprite);
  }
}

//change to catch animal image
void MultiSprite::catchAnimal(const std::string& n, const std::string& id) {
  if (n.find("Reverse") != std::string::npos) {
    setName(n.substr(0, n.find("Reverse")) + "CatchedReverse");
  }else{
    setName(n + "Catched");
  }
  images = (ImageFactory::getInstance().getImages (getName()));
  if(n == "Chicken" || n == "ChickenReverse"){
    setVelocityY(-fabs( getVelocityY()*1.05));
  }else{
    setVelocityY(-fabs( getVelocityY()*0.7));
  }
  catchedId = id;
}

//change to original image (without catching)
void MultiSprite::releaseAnimal(int check) {
  if(check == 0){
    setName(initialName);
    images = (ImageFactory::getInstance().getImages (getName()));
  }else catchedId = "";
}

void MultiSprite::draw() const { 
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) {
  if ( explosionEnd ) return;
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
      explosionEnd = true;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < -400) {
    setName(initialName);
    images = (ImageFactory::getInstance().getImages (getName()));
    catchedId = "";
    setVelocity(initialVelocity);
    setPosition(makeLocation(initialPosition[0], initialPosition[1]));
  }
  if ( getY() > 345) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
