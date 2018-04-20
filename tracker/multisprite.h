#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class ExplodingSprite;

class MultiSprite : public Drawable {
public:
  MultiSprite(const std::string&, const std::string&);
  MultiSprite(const MultiSprite&);
  ~MultiSprite();

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

  virtual void explode();
  virtual bool hasExploded() const { return explosionEnd; }

  void catchAnimal(const std::string&, const std::string&);
  void releaseAnimal(int);

  std::string getIdentity() const {
    return identity;
  }
  void setIdentity(const std::string id) { 
    identity = id;    
  }
  std::string getCatchedId() const {
    return catchedId;
  }
  void setCatchedId(const std::string id) { 
    catchedId = id;    
  }

protected:
  std::vector<Image *> images;
  ExplodingSprite* explosion;
  bool explosionEnd;
  std::string initialName;
  Vector2f initialVelocity;
  Vector2f initialPosition;
  std::string catchedId;
  std::string identity;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  Vector2f makeLocation(int, int) const;
  Vector2f makeVelocity(int, int) const;
  void advanceFrame(Uint32 ticks);
  MultiSprite& operator=(const MultiSprite&);
};
#endif
