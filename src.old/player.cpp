#include "common.h"
#include "player.h"

namespace Tots
{
  Player::Player(const std::string &name, Type type, Sprite *sprite, Behavior *behavior) : Entity(name, type, sprite, behavior)
  {
    m_xVelocity = m_yVelocity = 0;
    m_friction = FRICTION;
  }

  Player::~Player()
  {
  }

  void Player::handleTick()
  {
    m_x += m_xVelocity / FRAMES_PER_SECOND;
    m_y += m_yVelocity / FRAMES_PER_SECOND;

    // add friction
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

  void Player::keyLeft(bool depressed)
  {
    if(depressed && m_xVelocity > (-MAX_VELOCITY))
    {
      m_xVelocity -= ACCELERATION / FRAMES_PER_SECOND;
    }
  }

  void Player::keyRight(bool depressed)
  {
    if(depressed && m_xVelocity < MAX_VELOCITY)
    {
      m_xVelocity += ACCELERATION / FRAMES_PER_SECOND;
    }
  }

  void Player::keyUp(bool depressed)
  {
    if(depressed && m_yVelocity > (-MAX_VELOCITY))
    {
      m_yVelocity -= ACCELERATION / FRAMES_PER_SECOND;
    }
  }

  void Player::keyDown(bool depressed)
  {
    if(depressed && m_yVelocity < MAX_VELOCITY)
    {
      m_yVelocity += ACCELERATION / FRAMES_PER_SECOND;
    }
  }
}
