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

      void run(Subsystem *subsystem, SubsystemThread::Command command);

    protected:
      SDL_sem *m_threadSemaphore;
      SDL_atomic_t m_done;

      void waitThreadSemaphore() { SDL_SemWait(m_threadSemaphore); }
      void postThreadSemaphore() { SDL_SemPost(m_threadSemaphore); }

    private:
      WorkerThread **m_threads;
      size_t m_numThreads;

      HoggedThread **m_hoggedThreads;
      size_t m_numHoggedThreads;
  };
}

#endif
