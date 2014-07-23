#include "threadPool.h"

#include "gameState.h"
#include "subsystemThread.h"
#include "subsystem.h"

#include <cstdio>

#define MAX_THREADS 16

namespace tots {
  ThreadPool::ThreadPool(size_t numThreads) : m_numThreads(numThreads), m_hoggedThreads(0) {
    // initiate the SDL thread synchronization primitives
    m_threadSemaphore = SDL_CreateSemaphore(0);
    SDL_AtomicSet(&m_done, 0);

    // create an empty game state to share among the threads
    GameState gs;

    // create the threads
    assert(numThreads <= MAX_THREADS);
    m_threads = new WorkerThread*[MAX_THREADS];
    m_hoggedThreads = new HoggedThread*[MAX_THREADS];
    for(size_t i = 0; i < numThreads; ++i) {
      m_threads[i] = new WorkerThread(i, this, &gs);
    }
  }

  ThreadPool::~ThreadPool() {
    // join all the running threads and destroy the thread semaphore
    SDL_AtomicSet(&m_done, 1);
    for(size_t i = 0; i < m_numThreads; ++i) {
      delete m_threads[i];
    }
    for(size_t i = 0; i < m_numHoggedThreads; ++i) {
      delete m_hoggedThreads[i];
    }
    SDL_DestroySemaphore(m_threadSemaphore);
    delete[] m_threads;
    delete[] m_hoggedThreads;
  }

  void ThreadPool::run(Subsystem *subsystem, SubsystemThread::Command command) {
    // run with a hogged thread
    if(subsystem->hints() & Subsystem::HOG_THREAD) {
      if(subsystem->m_hoggedThread == NULL) {
        assert(m_numThreads >= 2);  // FIXME: come up with a strategy for increasing the number of threads when this assertion fails

        // create a thread for a hog
        assert(m_numHoggedThreads < MAX_THREADS);
        m_hoggedThreads[m_numHoggedThreads++] = new HoggedThread(subsystem->name(), this, &gs);
      }
      // run with the hogged thread
      assert(SDL_AtomicGet(&(subsystem->m_hoggedThread->m_free)));
      subsystem->m_hoggedThread->run(subsystem, init);
      return;
    }

    // wait for a free thread
    waitThreadSemaphore();

    // try using the same thread, to avoid cache misses
    if(subsystem->m_lastThread != NULL && subsystem->m_lastThread->isFree()) {
      subsystem->m_lastThread->run(subsystem, init);
      return;
    }

    // find the first free thread
    int threadIndex = -1;
    for(size_t i = 0; i < m_numThreads; ++i) {
      if(SDL_AtomicGet(&(m_threads[i]->m_free))) {
        threadIndex = i;
        break;
      }
    }
    assert(threadIndex != -1);

    // run the subsystem on that thread
    m_threads[threadIndex]->run(subsystem, init);
  }
}
