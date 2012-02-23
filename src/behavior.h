#ifndef BEHAVIOR_H_
#define BEHAVIOR_H_

#include <string>

namespace Tots
{
  class Entity;

  class Behavior
  {
    public:
      class State
      {
        public:
          State(const std::string &name);
          virtual ~State();

          std::string name() { return m_name; }

          virtual void tick(Entity *entity) = 0;

        private:
          std::string m_name;
      };

      enum StateFlag { SPAWNED = 0, IDLE, DESTROYED, FIRST_CUSTOM_STATE };

      Behavior(const std::string &name, size_t numberOfStates);
      ~Behavior();

      std::string name() { return m_name; }

      size_t numberOfStates() { return m_numberOfStates; }

      void addState(StateFlag flag, Behavior::State *state);

      void tick(Entity *entity);

    private:
      std::string m_name;
      size_t m_numberOfStates;  // FIXME: make the number of states dynamic
      Behavior::State **m_states;
  };
}

#endif
