#include "behavior.h"
#include "entity.h"
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
    m_states = new State*[numberOfStates];
    for(size_t i = 0; i < numberOfStates; i++)
    {
      m_states[i] = NULL;
    }
  }

  Behavior::~Behavior()
  {
  }

  /*!
   * Adds a state to this behavior and sets the flag that corresponds with that state.
   *
   * Behavior does not assume ownership of \a state. If you want \a state to be destroyed properly, register it with Level::addBehaviorState()
   *
   * \sa Level::addMovement() Level::getMovement()
   */
  void Behavior::addState(Behavior::StateFlag flag, Behavior::State *state)
  {
    // FIXME: shouldn't depend on a static array of states
    if(static_cast<size_t>(flag) >= m_numberOfStates)
      return;

    m_states[flag] = state;
  }

  void Behavior::tick(Entity *entity)
  {
    if(static_cast<size_t>(entity->currentState()) >= m_numberOfStates)
    {
      return;
    }
    if(m_states[entity->currentState()] == NULL)
    {
      return;
    }

    m_states[entity->currentState()]->tick(entity);
  }

  /*!
   * \class Behavior::State
   * The Behavior::State class determines how an entity moves and behaves. This is the class that should be inherited to actually move the entity around, give it attacks, etc.
   *
   * Some states might move an entity around based on some kind of path geometry, or based on a lua script.
   * \sa Behavior::addState()
   */
  Behavior::State::State(const std::string &name)
  {
    m_name = name;
  }

  Behavior::State::~State()
  {
  }
}
