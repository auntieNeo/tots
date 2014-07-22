#include "subsystem.h"

#include <cstdio>

namespace tots {
  Subsystem::Subsystem() {
  }

  Subsystem::~Subsystem() {
  }

  void Subsystem::update(const GameState *state) {
    // TODO: determine and update the delta time in m_dt
    m_update(state);
  }
}
