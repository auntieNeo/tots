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

      void init(Subsystem *subsystem);
      void run(Subsystem *subsystem);

    protected:
      SDL_sem *m_threadSemaphore;
      SDL_atomic_t m_done;

    private:
      SubsystemThread **m_threads;
      size_t m_numThreads;

      SubsystemThread **m_hoggedThreads;
      size_t m_numHoggedThreads;
  };
}

#endif
