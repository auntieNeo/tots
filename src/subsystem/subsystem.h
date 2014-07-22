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

      void update(const GameState *state);

    protected:
      virtual void m_update(const GameState *state) = 0;
      size_t m_dt;

    private:
      EventQueue *m_input, *m_output;
  };
}

#endif
