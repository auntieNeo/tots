#ifndef PLAYER_MOVEMENT
#define PLAYER_MOVEMENT

#include <allegro5/allegro.h>

#include "behavior.h"
#include "common.h"

namespace Tots
{
  class PlayerState : public Behavior::State
  {
    public:
      PlayerState(const std::string &name, double maxVelocity, double acceleration, double friction, int upKey, int downKey, int leftKey, int rightKey);
      ~PlayerState();

      void tick(Entity *entity);

    private:
      double m_maxVelocity, m_acceleration, m_friction;
      int m_upKey, m_downKey, m_leftKey, m_rightKey;
      double m_xVelocity, m_yVelocity;
      ALLEGRO_KEYBOARD_STATE *m_keyboardState;
  };
}

#endif
