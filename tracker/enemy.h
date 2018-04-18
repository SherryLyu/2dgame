#include <iostream>
#include <string>
#include <list>
#include "multisprite.h"


class EnemyController {
public:
  EnemyController(const std::string& name);
  EnemyController(const EnemyController& e);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void enemy();

private:
  std::string enemyName;
  std::list<Enemy> enemieslist;
  std::list<Enemy> freelist;

  Enemy& operator=(const Enemy&);
};
