#include <allegro5/allegro_primitives.h>

#include "enemy.h"

namespace Tots
{ 
  Enemy::Enemy(const std::string &name, Type type, Sprite *sprite, Behavior *behavior) : Entity(name, type, sprite, behavior)
  {
    /*
    m_path = new Path(x, y, 0.5);
    m_path->quadTo(150, 0, 150, 150, 0.5);
    m_path->quadTo(150, 350, 0, 350, 2);
    */
  }

  Enemy::~Enemy()
  {
//    delete m_path;
  }

  void Enemy::handleTick()
  {
  }
}
