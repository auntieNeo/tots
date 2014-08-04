#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "../common.h"

#include "subsystemThread.h"

#include <SDL_thread.h>

namespace tots {
  class Subsystem;
  class SubsystemThread;
  class Task;
  class ThreadPool {
    friend class SubsystemThread;
    friend class WorkerThread;
    public:
      ThreadPool(size_t numThreads, const GameState *gs);
      ~ThreadPool();

      void registerSubsystems(Subsystem **subsystems, size_t numSubsystems);
      void run(Task &task);
      bool tryRun(Task &task);
      void flush();

    private:
      SubsystemThread **m_threads;
      size_t m_numThreads;
      SDL_sem *m_readySemaphore;

      void waitReady() { SDL_SemWait(m_readySemaphore); }
      bool tryWaitReady();

      void m_run(Task &task);
  };
}

#endif
