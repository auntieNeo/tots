#include "threadPool.h"

#include "subsystemThread.h"

#include <cstdio>

namespace tots {
  ThreadPool::ThreadPool(size_t numThreads) : m_numThreads(numThreads) {
    m_threads = new SubsystemThread*[numThreads];
    char *threadName = new char[256];
    for(size_t i = 0; i < numThreads; ++i) {
      snprintf(threadName, 256, "Subsystem Thread %02ld", i);
      m_threads[i] = new SubsystemThread(threadName);
    }
    delete threadName;
  }

  ThreadPool::~ThreadPool() {
  }

  void ThreadPool::run(Subsystem *subsystem) {
    // TODO: wait for a free thread, run subsystem, loop
  }
}
