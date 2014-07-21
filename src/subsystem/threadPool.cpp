#include "threadPool.h"

#include "gameState.h"
#include "subsystemThread.h"

#include <cstdio>

namespace tots {
  ThreadPool::ThreadPool(size_t numThreads) : m_numThreads(numThreads) {
    // initiate the SDL thread synchronization primitives
    assert(numThreads < sizeof(SDL_atomic_t) * 8);
    SDL_AtomicSet(&m_freeThreads, (1 << numThreads) - 1);
    m_threadSemaphore = SDL_CreateSemaphore(numThreads);

    // create an empty game state to share among the threads
    GameState gs;

    // create the threads
    m_threads = new SubsystemThread*[numThreads];
    for(size_t i = 0; i < numThreads; ++i) {
      m_threads[i] = new SubsystemThread(i, this, &gs);
    }
  }

  ThreadPool::~ThreadPool() {
    // join all the running threads and destroy the thread semaphore
    SDL_AtomicSet(&m_done, 1);
    for(size_t i = 0; i < m_numThreads; ++i) {
      delete m_threads[i];
    }
    SDL_DestroySemaphore(m_threadSemaphore);
    delete[] m_threads;
  }

  void ThreadPool::run(Subsystem *subsystem) {
    // wait for a free thread
    SDL_SemWait(m_threadSemaphore);

    // find the first free thread
    int threadIndex = -1;
    for(int i = 0; i < m_numThreads; ++i) {
      if(SDL_AtomicGet(&(m_threads->m_free))) {
        threadIndex = i;
        break;
      }
    }
    assert(threadIndex != -1);

    // run the subsystem on that thread
    m_threads[threadIndex]->run(subsystem);
  }
}
