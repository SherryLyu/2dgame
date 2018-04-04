#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwowaymultiSprite : public Drawable {
public:
  TwowaymultiSprite(const std::string&);
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
  std::string getName()  const { 
    return currentname; 
  }
protected:
  std::vector<Image *> images;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;
  std::string currentname;

  void advanceFrame(Uint32 ticks);
  TwowaymultiSprite& operator=(const TwowaymultiSprite&);
};
#endif
