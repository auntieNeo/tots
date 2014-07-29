#include "threadPool.h"

#include "gameState.h"
#include "subsystemThread.h"
#include "subsystem.h"

#include <cstdio>

#define MAX_THREADS 16

namespace tots {
  ThreadPool::ThreadPool(size_t numThreads, const GameState *gs) : m_numThreads(numThreads) {
    // initiate the SDL thread synchronization primitives
    m_readySemaphore = SDL_CreateSemaphore(0);

    // create the threads
    assert(numThreads <= MAX_THREADS);
    m_threads = new SubsystemThread*[MAX_THREADS];
    char *threadName = new char[256];
    for(size_t i = 0; i < numThreads; ++i) {
      sprintf(threadName, "Pool Thread %02ld", i);
      m_threads[i] = new SubsystemThread(threadName, gs, m_readySemaphore);
    }
    delete[] threadName;
  }

  ThreadPool::~ThreadPool() {
    // join all the running threads and destroy the thread semaphore
    for(size_t i = 0; i < m_numThreads; ++i) {
      delete m_threads[i];
    }
    SDL_DestroySemaphore(m_readySemaphore);
    delete[] m_threads;
  }

  void ThreadPool::registerSubsystems(Subsystem **subsystems, size_t numSubsystems) {
  }

  void ThreadPool::run(Subsystem *subsystem, SubsystemThread::Command command) {
    // FIXME: These need to be queued. This method should never, ever block.
    // wait for a free thread
    waitReady();  // FIXME: should not block here

    // try using the same thread, to avoid cache misses
    if(subsystem->m_lastThread != NULL && subsystem->m_lastThread->isFree()) {
      subsystem->m_lastThread->run(subsystem, command);
      return;
    }

    // find the first free thread
    int threadIndex = -1;
    for(size_t i = 0; i < m_numThreads; ++i) {
      if(m_threads[i]->isFree()) {
        threadIndex = i;
        break;
      }
    }
    assert(threadIndex != -1);

    // run the subsystem on that thread
    m_threads[threadIndex]->run(subsystem, command);
  }

  void ThreadPool::flush() {
    // TODO: signal subsystems to run based on their priority and what threads are available
  }
}
