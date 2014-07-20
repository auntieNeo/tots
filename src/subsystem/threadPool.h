#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include "../common.h"

namespace tots {
  class Subsystem;
  class SubsystemThread;
  class ThreadPool {
    public:
      ThreadPool(size_t numThreads);
      ~ThreadPool();

      void run(Subsystem *subsystem);

    private:
      SubsystemThread **m_threads;
      size_t m_numThreads;
  };
}

#endif
