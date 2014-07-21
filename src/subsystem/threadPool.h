#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "../common.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  class Subsystem;
  class SubsystemThread;
  class ThreadPool {
    friend class SubsystemThread;
    public:
      ThreadPool(size_t numThreads);
      ~ThreadPool();

      void run(Subsystem *subsystem);

    protected:
      SDL_atomic_t m_freeThreads;
      SDL_sem *m_threadSemaphore;

    private:
      SubsystemThread **m_threads;
      size_t m_numThreads;
  };
}

#endif
