#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
  ~Player();
  
  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  bool getJumpStatus()  const {
    return jumpingStatus;
  }
  bool getHitStatus()  const {
    return hitStatus;
  }

  void right();
  void left();
  void jump();
  void down();
  void hit();
  void stop();
  void fall();
  void stand();
  
  virtual void explode();

private:
  enum MODE {STAND, JUMP, DOWN, WALKRIGHT, WALKLEFT, HIT, FALL};
  std::vector<Image *> images;
  ExplodingSprite* explosion;
  bool jumpingStatus;
  bool hitStatus;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  MODE lastMode;
  MODE currentMode;

  Vector2f initialVelocity;
  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
};
#endif
