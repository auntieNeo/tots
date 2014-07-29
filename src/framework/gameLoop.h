#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include "../common.h"

/**
 * GameLoop implements a non-locking game loop that shares data between
 * subsystem threads using an aggregate command queue and multiple copies of
 * the GameState object.
 */

namespace tots {
  class GameState;
  class AggregateQueue;
  class ThreadPool;
  class Subsystem;
  class GameLoop {
    public:
      GameLoop(Subsystem **subsystems, size_t numSubsystems);
      ~GameLoop();

      void run();

    private:
      GameState *m_state;
      AggregateQueue *m_queue;  // TODO: make this a template

      Subsystem **m_subsystems;
      size_t m_numSubsystems;

      ThreadPool *m_threads;
  };
}

#endif
