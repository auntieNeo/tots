#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include <string>

#include "entity.h"

namespace Tots
{
  class Movement;

  class Behavior
  {
    public:
      Behavior(const std::string &name, size_t numberOfStates);
      ~Behavior();

      std::string name() { return m_name; }

      size_t numberOfStates() { return m_numberOfStates; }

      void setMovement(Entity::State state, Movement *movement);

      void tick(Entity *entity);

    private:
      std::string m_name;
      size_t m_numberOfStates;
      Movement **m_movement;
      // TODO add lua callbacks
  };
}

#endif
