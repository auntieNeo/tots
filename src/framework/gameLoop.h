#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include "../common.h"
#include "../utility/priorityQueue.h"
#include "subsystem.h"

/**
 * GameLoop implements a non-locking game loop that shares data between
 * subsystem threads using an aggregate command queue and multiple copies of
 * the GameState object.
 */

namespace tots {
  class GameState;
  class AggregateQueue;
  class ThreadPool;

  /**
   * The GameLoop class represents the outermost loop of the Tots engine. The
   * GameLoop's primary task is to dispatch the various subsystems on one or
   * more threads.
   *
   * Games that use the Tots engine must construct a GameLoop object with a
   * list of Subsystem objects that will be used for the duration of the
   * GameLoop. The game can set different parameters on the GameLoop to affect
   * its behavior, such as the framerate, the number of frames to skip, and the
   * period of the game clock.
   *
   * Additionally, through the Subsystem class interface, each Subsystem object
   * advertises the period on which it must be scheduled for updates, as well as
   * hint flags that affect the GameLoop's behavior. Descriptions of these
   * attributes and flags are described in the Subsystem class' API.
   *
   * The operation of the GameLoop is subtle and complicated. See these
   * resources:
   *
   * <a href="http://gameprogrammingpatterns.com/game-loop.html">Game Loop - Game Programming Patterns</a>
   *
   * <a href="http://gafferongames.com/game-physics/fix-your-timestep/">Fix Your Timestep! - gafferongames.com</a>
   */
  class GameLoop {
    public:
      GameLoop(Subsystem **subsystems, size_t numSubsystems);
      ~GameLoop();

      void run();

    private:
      class Task {
        public:
          Task(Subsystem *subsystem, Subsystem::Command command) : m_subsystem(subsystem), m_command(command) {}
          ~Task() {}

          Subsystem *subsystem() { return m_subsystem; }
          Subsystem::Command command() { return m_command; }

        private:
          Subsystem *m_subsystem;
          Subsystem::Command m_command;
      };

      GameState *m_state;
//      AggregateQueue<Message> *m_messageQueue;
      typedef MinPriorityQueue< uint64_t, Task > TaskQueue;
      TaskQueue *m_taskQueue;

      Subsystem **m_subsystems;
      size_t m_numSubsystems;

      ThreadPool *m_threads;

      void m_scheduleTask(Subsystem *subsystem, Subsystem::Command command, uint32_t gameTime, Subsystem::Priority priority = Subsystem::SUBSYSTEM_PRIORITY);
  };
}

#endif
