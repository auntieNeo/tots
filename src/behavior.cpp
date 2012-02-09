#include "behavior.h"
#include "movement.h"

#include <iostream>
using namespace std;

namespace Tots
{
  /*!
   * \class Behavior
   * The behavior class stores the movements and Lua callbacks to be used for each state of an entity. Instantiated behaviors should be stored in Level and are, in most cases, shared among many entities.
   * \sa Level::addBehavior()
   */
  Behavior::Behavior(const std::string &name, size_t numberOfStates)
  {
    m_name = name;
    m_numberOfStates = numberOfStates;
    m_movement = new Movement*[numberOfStates];
    for(size_t i = 0; i < numberOfStates; i++)
    {
      m_movement[i] = NULL;
    }
  }

  Behavior::~Behavior()
  {
  }

  /*!
   * Sets the movement to be used for the specified state.
   *
   * Behavior does not assume ownership of \a movement. If you want \a movement to be destroyed properly, register it with Level::addMovement()
   *
   * \sa Level::addMovement() Level::getMovement()
   */
  void Behavior::setMovement(Entity::State state, Movement *movement)
  {
    if(static_cast<size_t>(state) >= m_numberOfStates)
      return;

    m_movement[state] = movement;
  }

  void Behavior::tick(Entity *entity)
  {
    if(static_cast<size_t>(entity->state()) >= m_numberOfStates)
    {
//      cout << "state is too high" << endl;
      return;
    }
    if(m_movement[entity->state()] == NULL)
    {
//      cout << "no movement was set" << endl;
      return;
    }
//    cout << "should be working" << endl;

    m_movement[entity->state()]->tick(entity);
  }
}
