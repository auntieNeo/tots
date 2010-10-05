#ifndef PLAYER_H_
#define PLAYER_H_

#include "entity.h"

namespace Tots
{ 
  class Player : public Entity
  {
    public:
      Player(const std::string &name, Type type, Sprite *sprite, Behavior *behavior);
      ~Player();

      void handleTick();

      void keyLeft(bool depressed);
      void keyRight(bool depressed);
      void keyUp(bool depressed);
      void keyDown(bool depressed);

    private:
      double m_xVelocity, m_yVelocity;
      double m_friction;
  };
}

#endif
