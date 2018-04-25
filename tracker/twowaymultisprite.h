#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwowaymultiSprite : public Drawable {
public:
  TwowaymultiSprite(const std::string&, const std::string&);
  TwowaymultiSprite(const TwowaymultiSprite&);

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

  std::string getIdentity() const {
    return identity;
  }
  void setIdentity(const std::string id) { 
    identity = id;    
  }
  std::string getCatcherId() const {
    return catcherId;
  }
  void setCatcherId(const std::string id) { 
    catcherId = id;    
  }
  void setVisible(const bool b) { 
    visible = b;   
  }
  void setReleasing(const bool b) { 
    releasing = b;   
  }
protected:
  std::vector<Image *> images;
  std::string catcherId;
  std::string identity;
  bool visible;
  bool releasing;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  float originalY;
  float originalVX;

  Vector2f makeVelocity(int, int) const;
  void advanceFrame(Uint32 ticks);
  TwowaymultiSprite& operator=(const TwowaymultiSprite&);
};
#endif
