#ifndef LOGGING_H_
#define LOGGING_H_

#include "../framework/subsystem.h"

namespace tots {
  class Logging : public Subsystem {
    public:
      Logging();
      ~Logging();

    protected:
      virtual void m_init(const GameState *state);
      virtual void m_update(const GameState *state);
      virtual void m_close(const GameState *state);
  };
}

#endif
