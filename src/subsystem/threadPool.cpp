#include "threadPool.h"

#include "subsystemThread.h"

#include <cstdio>

namespace tots {
  ThreadPool::ThreadPool(size_t numThreads) : m_numThreads(numThreads) {
    // initiate the SDL thread synchronization primitives
    assert(numThreads < sizeof(SDL_atomic_t) * 8);
    SDL_AtomicSet(&m_freeThreads, (1 << numThreads) - 1)
    m_threadSemaphore = SDL_CreateSemaphore(numThreads);

    m_threads = new SubsystemThread*[numThreads];
    for(size_t i = 0; i < numThreads; ++i) {
      m_threads[i] = new SubsystemThread(i, this);
    }
  }

  ThreadPool::~ThreadPool() {
    // join all the running threads and destroy the thread semaphore
    for(size_t i = 0; i < m_numThreads; ++i) {
      delete m_threads[i];
    }
    SDL_DestroySemaphore(m_threadSemaphore);
    delete[] m_threads;
  }

  void ThreadPool::run(Subsystem *subsystem) {
    // TODO: wait for a free thread, run subsystem, loop
  }
}
