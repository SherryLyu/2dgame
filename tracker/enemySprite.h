#include <string>
#include <iostream>
#include <list>
#include "multisprite.h"
#include "enemy.h"

class EnemySprite : public MultiSprite {
public:
  EnemySprite(const std::string& n);
  EnemySprite(const EnemySprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void enemy();

private:
  std::string enemyName;
  std::list<Enemy> enemieslist;
  std::list<Enemy> freelist;
  float minSpeed;
  float enemyInterval;
  float timeSinceLastFrame;
  EnemySprite& operator=(const EnemySprite&);
};
