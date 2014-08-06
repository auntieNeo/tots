#include "physics.h"

using namespace tots::framework;

namespace tots {
  Physics::Physics() {
  }

  Physics::~Physics() {
  }

  /**
   * The m_init() method implements the abstract method Subsystem::m_init() to
   * initialize resources used by the Physics subsystem.
   */
  void Physics::m_init(const GameState *state) {
  }

  /**
   * The m_update() method implements the abstract Subsystem::m_update() method
   * in order to advance the physics simulation with every update.
   */
  void Physics::m_update(const GameState *state) {
  }

  /**
   * The m_close() method implements the abstract Subsystem::m_close() method
   * to free resources allocated by the Physics subsystem.
   */
  void Physics::m_close(const GameState *state) {
  }
}
