#include "threadPool.h"

#include "../log.h"
#include "gameState.h"
#include "subsystemThread.h"
#include "subsystem.h"

#include <cstdio>
#include <SDL_thread.h>
#include <SDL_mutex.h>

#define MAX_THREADS 16

namespace tots { namespace framework {
  ThreadPool::ThreadPool(size_t numThreads, const GameState *gs, ThreadSignal *signal) :
    m_numThreads(numThreads), m_signal(signal) {
    // create the threads
    assert(numThreads <= MAX_THREADS);
    m_threads = new SubsystemThread*[MAX_THREADS];
    char *threadName = new char[256];
    for(size_t i = 0; i < numThreads; ++i) {
      sprintf(threadName, "Pool Thread %02ld", i);
      m_threads[i] = new SubsystemThread(threadName, gs, m_signal);
    }
    delete[] threadName;
  }

  ThreadPool::~ThreadPool() {
    // join all the running threads and destroy the thread semaphore
    for(size_t i = 0; i < m_numThreads; ++i) {
      delete m_threads[i];
    }
    delete[] m_threads;
  }

  bool ThreadPool::tryRun(Task &task) {
    // wait for a free thread; non-blocking
    // try using the same thread, to avoid cache misses
    if(task.lastThread() != NULL && task.lastThread()->isFree()) {
      task.lastThread()->run(task);
      return true;
    }

    // find the first free thread
    int threadIndex = -1;
    for(size_t i = 0; i < m_numThreads; ++i) {
      if(m_threads[i]->isFree()) {
        threadIndex = i;
        break;
      }
    }
    if(threadIndex == -1)
      return false;  // there weren't any free threads

    // run the subsystem on that thread
    m_threads[threadIndex]->run(task);
    return true;
  }
} }
