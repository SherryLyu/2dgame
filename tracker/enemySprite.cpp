#include "enemysprite.h"
#include "gamedata.h"

EnemySprite::EnemySprite(const std::string& name) :
  MultiSprite(name),
  enymyName( Gamedata::getInstance().getXmlStr(name+"/bird") ),
  enymies(),
  minSpeed( Gamedata::getInstance().getXmlInt(enymyName+"/speedX") ),
  enymyInterval(Gamedata::getInstance().getXmlInt(enymyName+"/interval")),
  timeSinceLastFrame(0)
{ }

EnemySprite::EnemySprite(const EnemySprite& s) :
  Sprite(s),
  enymyName(s.enymyName),
  enymies(s.enymies),
  minSpeed(s.minSpeed),
  enymyInterval(s.enymyInterval),
  timeSinceLastFrame(s.timeSinceLastFrame)
{ }

void EnemySprite::enemy() { 
  if ( timeSinceLastFrame < enymyInterval ) return;
  float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
  if ( freelist.empty() ) {
    Enemy enemy(getName());
    enemylist.push_back( enemy );
  }else{
    Enemy enemy = freelist.front();
    enemy.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
    enemy.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
    enemylist.push_back( enemy);
  }
  /*float deltaX = getScaledWidth();
  float deltaY = getScaledHeight()/2;
  // I need to add some minSpeed to velocity:
  Enemy enymy(enymyName);
  enymy.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
  enymy.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
  enymies.push_back( enymy );*/
  timeSinceLastFrame = 0;
}

void EnemySprite::draw() const { 
  Sprite::draw();
  for ( const Enemy& enymy : enymies ) {
    enymy.draw();
  }
}

void EnemySprite::update(Uint32 ticks) { 
	timeSinceLastFrame += ticks;
  MultiSprite::update(ticks);
  for ( Enemy& enymy : enymies ) {
    enymy.update(ticks);
  }
}

