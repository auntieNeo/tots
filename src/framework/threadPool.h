#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "../common.h"

#include "subsystemThread.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  class Subsystem;
  class SubsystemThread;
  class WorkerThread;
  class HoggedThread;
  class ThreadPool {
    friend class SubsystemThread;
    friend class WorkerThread;
    public:
      ThreadPool(size_t numThreads);
      ~ThreadPool();

      void registerSubsystems(Subsystem **subsystems, size_t numSubsystems);
      void run(Subsystem *subsystem, SubsystemThread::Command command);

    protected:
      void waitThreadSemaphore() { SDL_SemWait(m_threadSemaphore); }
      void postThreadSemaphore() { SDL_SemPost(m_threadSemaphore); }

    private:
      // create an empty game state to share among the threads
      GameState *m_gs;

      WorkerThread **m_threads;
      size_t m_numThreads;
      SDL_sem *m_threadSemaphore;

      HoggedThread **m_hoggedThreads;
      size_t m_numHoggedThreads;
  };
}

#endif
