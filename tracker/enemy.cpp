#include <iostream>
#include <cmath>
#include "gamedata.h"
#include "enemy.h"

Enemy::Enemy(const std::string& name) :
    MultiSprite(name), 
    enemieslist(),
  	freelist()
{ }

Enemy::Enemy(const Enemy& e) : 
	MultiSprite(e), 
    enemieslist(e.enemieslist),
  	freelist(e.freelist)
{ }

void Enemy::enemy() {
	if ( freelist.empty() ) {
	    Enemy enemy(getName());
	    enemylist.push_back( enemy );
	}else{
		Enemy enemy = freelist.front();
		enemy.setPosition( getPosition() +Vector2f(deltaX, deltaY) );
		enemy.setVelocity( getVelocity() + Vector2f(minSpeed, 0) );
		enemylist.push_back( enemy);
	}
}


void Enemy::draw() const {
	for ( const Enemy& enymy : enymies ) {
	    enymy.draw();
	}
}

void Enemy::update(Uint32 ticks) {

}

 
void Enemy::update(Uint32 ticks) { 
  Vector2f pos = getPosition();
  Sprite::update(ticks);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
}

