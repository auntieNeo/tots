#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "../framework/subsystem.h"

namespace tots {
  /**
   * The Physics subsystem is responsible for simulating 2D kinematics in the
   * Tots engine. This includes simulating velocity, acceleration, gravity, and
   * angular velocity, among other motion-related things. Note that this does
   * not include collision, which is handled in the separate Collision
   * subsystem.
   *
   * The Physics subsystem is also responsible for passing along simple "jump"
   * motion that ignores physics, such as motion that comes from an AI routine
   * or from a script. This is done so that other subsystems don't "fight" with
   * Physics subsystem for control over motion. The Physics subsystem is always
   * given the final word when it comes to any motion events. This allows the
   * Physics subsystem to do stuff like factor in velocity, universal gravity,
   * or other things that might affect an object's ultimate trajectory, if it
   * so chooses.
   */
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
