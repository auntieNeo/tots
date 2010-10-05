#ifndef ENEMY_H_
#define ENEMY_H_

#include "entity.h"
#include "path.h"

namespace Tots
{ 
  class Enemy : public Entity
  {
    public:
      Enemy(const std::string &name, Type type, Sprite *sprite, Behavior *behavior);
      ~Enemy();

      virtual void handleTick();
  };
}

#endif
