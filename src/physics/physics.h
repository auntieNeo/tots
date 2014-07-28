#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "../framework/subsystem.h"

namespace tots {
  class Physics : public Subsystem {
    public:
      Physics();
      ~Physics();

      const char *name() const { return "Physics Subsystem"; }

    protected:
      void m_init(const GameState *state);
      void m_update(const GameState *state);
      void m_close(const GameState *state);
  };
}

#endif
