#ifndef SUBSYSTEM_H_
#define SUBSYSTEM_H_

#include "../common.h"

namespace tots {
  class GameState;
  class EventQueue;
  class Subsystem {
    public:
      Subsystem();
      virtual ~Subsystem();

      virtual void update(const GameState *state);

    private:
      EventQueue *m_input, *m_output;
  };
}

#endif
