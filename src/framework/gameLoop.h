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
