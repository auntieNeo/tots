#include "entity.h"
#include "playerMovement.h"

namespace Tots
{
  PlayerMovement::PlayerMovement(const std::string &name, double maxVelocity, double acceleration, double friction, int upKey, int downKey, int leftKey, int rightKey) : Movement(name)
  {
    m_maxVelocity = maxVelocity;
    m_acceleration = acceleration;
    m_friction = friction;
    m_upKey = upKey;
    m_downKey = downKey;
    m_leftKey = leftKey;
    m_rightKey = rightKey;
    m_keyboardState = new ALLEGRO_KEYBOARD_STATE;
  }

  PlayerMovement::~PlayerMovement()
  {
    delete m_keyboardState;
  }

  void PlayerMovement::tick(Entity *entity)
  {
    // read the keyboard/gamepad
    al_get_keyboard_state(m_keyboardState);
    if(al_key_down(m_keyboardState, m_upKey))
    {
      if(m_yVelocity > (-MAX_VELOCITY))
      {
        m_yVelocity -= ACCELERATION / FRAMES_PER_SECOND;
      }
    }
    if(al_key_down(m_keyboardState, m_downKey))
    {
      if(m_yVelocity < MAX_VELOCITY)
      {
        m_yVelocity += ACCELERATION / FRAMES_PER_SECOND;
      }
    }
    if(al_key_down(m_keyboardState, m_leftKey))
    {
      if(m_xVelocity > (-MAX_VELOCITY))
      {
        m_xVelocity -= ACCELERATION / FRAMES_PER_SECOND;
      }
    }
    if(al_key_down(m_keyboardState, m_rightKey))
    {
      if(m_xVelocity < MAX_VELOCITY)
      {
        m_xVelocity += ACCELERATION / FRAMES_PER_SECOND;
      }
    }

    // displace player based on velocity
    entity->setX(entity->x() + m_xVelocity / FRAMES_PER_SECOND);
    entity->setY(entity->y() + m_yVelocity / FRAMES_PER_SECOND);

    // add friction to velocity
    if(m_xVelocity > 0)
    {
      m_xVelocity -= m_friction / FRAMES_PER_SECOND;
      if(m_xVelocity < 0)
      {
        m_xVelocity = 0;
      }
    }
    else if(m_xVelocity < 0)
    {
      m_xVelocity += m_friction / FRAMES_PER_SECOND;
      if(m_xVelocity > 0)
      {
        m_xVelocity = 0;
      }
    }
    if(m_yVelocity > 0)
    {
      m_yVelocity -= m_friction / FRAMES_PER_SECOND;
      if(m_yVelocity < 0)
      {
        m_yVelocity = 0;
      }
    }
    else if(m_yVelocity < 0)
    {
      m_yVelocity += m_friction / FRAMES_PER_SECOND;
      if(m_yVelocity > 0)
      {
        m_yVelocity = 0;
      }
    }
  }
}
