#include "subsystem.h"

#include <cstdio>

namespace tots {
  Subsystem::Subsystem() : m_lastThread(NULL), m_hoggedThread(NULL) {
  }

  Subsystem::~Subsystem() {
  }

  void Subsystem::init(const GameState *state) {
    // TODO: initialize something I guess
    m_init(state);
  }

  void Subsystem::update(const GameState *state) {
    // TODO: determine and update the delta time in m_dt
    m_update(state);
  }
}
