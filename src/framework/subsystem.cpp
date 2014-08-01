#include "subsystem.h"

#include <cstdio>

namespace tots {
  /**
   * The Subsystem class embodies a system in the Tots engine. Subsystems can
   * be run in multiple threads, and they must communicate using Message
   * queues. This design serves to decouple different systems in the Tots
   * engine, and also facilitates message passing among subsystem threads
   * without excessive thread locking.
   */
  Subsystem::Subsystem() : m_lastThread(NULL) {
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

  void Subsystem::close(const GameState *state) {
    // TODO: close stuff I guess
    m_close(state);
  }
}
