#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "../common.h"

#include "subsystemThread.h"

#include <SDL_thread.h>

namespace tots {
  class Subsystem;
  class SubsystemThread;
  class Task;
  class ThreadSignal;
  class ThreadPool {
    friend class SubsystemThread;
    friend class WorkerThread;
    public:
      ThreadPool(size_t numThreads, const GameState *gs, ThreadSignal *signal);
      ~ThreadPool();

      void registerSubsystems(Subsystem **subsystems, size_t numSubsystems);

//      void run(Task &task);  // GameLoop handles all the blocking now
      bool tryRun(Task &task);

    private:
      SubsystemThread **m_threads;
      size_t m_numThreads;
      ThreadSignal *m_signal;
  };
}

#endif
