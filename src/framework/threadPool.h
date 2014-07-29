#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "../common.h"

#include "subsystemThread.h"

#include <SDL_thread.h>

namespace tots {
  class Subsystem;
  class SubsystemThread;
  class ThreadPool {
    friend class SubsystemThread;
    friend class WorkerThread;
    public:
      ThreadPool(size_t numThreads, const GameState *gs);
      ~ThreadPool();

      void registerSubsystems(Subsystem **subsystems, size_t numSubsystems);
      void run(Subsystem *subsystem, SubsystemThread::Command command);
      void flush();

    protected:
      void waitReady() { SDL_SemWait(m_readySemaphore); }

    private:
      SubsystemThread **m_threads;
      size_t m_numThreads;
      SDL_sem *m_readySemaphore;
  };
}

#endif
