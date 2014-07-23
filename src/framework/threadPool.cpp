#include "threadPool.h"

#include "gameState.h"
#include "subsystemThread.h"
#include "subsystem.h"
#include "workerThread.h"
#include "hoggedThread.h"

#include <cstdio>

#define MAX_THREADS 16

namespace tots {
  ThreadPool::ThreadPool(size_t numThreads) : m_numThreads(numThreads), m_hoggedThreads(0) {
    // initiate the SDL thread synchronization primitives
    m_threadSemaphore = SDL_CreateSemaphore(0);

    m_gs = new GameState();

    // create the threads
    assert(numThreads <= MAX_THREADS);
    m_threads = new WorkerThread*[MAX_THREADS];
    m_hoggedThreads = new HoggedThread*[MAX_THREADS];
    for(size_t i = 0; i < numThreads; ++i) {
      m_threads[i] = new WorkerThread(i, this, m_gs);
    }
  }

  ThreadPool::~ThreadPool() {
    // join all the running threads and destroy the thread semaphore
    for(size_t i = 0; i < m_numThreads; ++i) {
      delete m_threads[i];
    }
    for(size_t i = 0; i < m_numHoggedThreads; ++i) {
      delete m_hoggedThreads[i];
    }
    SDL_DestroySemaphore(m_threadSemaphore);
    delete[] m_threads;
    delete[] m_hoggedThreads;
    delete m_gs;
  }

  void ThreadPool::registerSubsystems(Subsystem **subsystems, size_t numSubsystems) {
    for(size_t i = 0; i < numSubsystems; ++i) {
      if(subsystems[i]->hints() & Subsystem::HOG_THREAD) {
        assert(subsystems[i]->m_hoggedThread == NULL);

        // create a thread for a hog
        assert(m_numHoggedThreads < MAX_THREADS);
        subsystems[i]->m_hoggedThread = m_hoggedThreads[m_numHoggedThreads++] = new HoggedThread(subsystems[i]->name(), m_gs);
        // change the current subsystem (it shouldn't ever change)
        subsystems[i]->m_hoggedThread->setCurrentSubsystem(subsystems[i]);
      }
    }
  }

  void ThreadPool::run(Subsystem *subsystem, SubsystemThread::Command command) {
    if(subsystem->m_hoggedThread != NULL) {
      // run with the hogged thread
      subsystem->m_hoggedThread->run(subsystem, command);
      return;
    }

    // wait for a free thread
    waitThreadSemaphore();

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
}
