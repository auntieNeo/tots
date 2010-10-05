#ifndef PLAYER_MOVEMENT
#define PLAYER_MOVEMENT

#include <allegro5/allegro.h>

#include "common.h"
#include "movement.h"

namespace Tots
{
  class PlayerMovement : public Movement
  {
    public:
      PlayerMovement(const std::string &name, double maxVelocity, double acceleration, double friction, int upKey, int downKey, int leftKey, int rightKey);
      ~PlayerMovement();

      void tick(Entity *entity);

    private:
      double m_maxVelocity, m_acceleration, m_friction;
      int m_upKey, m_downKey, m_leftKey, m_rightKey;
      double m_xVelocity, m_yVelocity;
      ALLEGRO_KEYBOARD_STATE *m_keyboardState;
  };
}

#endif
