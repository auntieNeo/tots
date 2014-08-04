#include "threadPool.h"

#include "../log.h"
#include "gameState.h"
#include "subsystemThread.h"
#include "subsystem.h"

#include <cstdio>
#include <SDL_thread.h>
#include <SDL_mutex.h>

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

  void ThreadPool::run(Task &task) {
    // wait for a free thread; blocking
    waitReady();
    m_run(task);
  }

  bool ThreadPool::tryRun(Task &task) {
    // wait for a free thread; non-blocking
    if(tryWaitReady()) {
      m_run(task);
      return true;
    }
    return false;
  }

  bool ThreadPool::tryWaitReady() {
    int response = SDL_SemTryWait(m_readySemaphore);
    if(response == 0)
      return true;
    else if(response == SDL_MUTEX_TIMEDOUT)
      return false;
    if(response < 0) {
      log_SDL_error("Error waiting for semaphore");
      exit(1);  // FIXME: abort properly
    }
    assert(false);  // if we get here, there's an SDL flag I don't know about
    return false;
  }

  void ThreadPool::m_run(Task &task) {
    // NOTE: this routine assumes that we have locked the semaphore and that a thread is available

    // try using the same thread, to avoid cache misses
    if(task.lastThread() != NULL && task.lastThread()->isFree()) {
      task.lastThread()->run(task);
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
    assert(threadIndex != -1);  // m_run() was called without waiting for free threads

    // run the subsystem on that thread
    m_threads[threadIndex]->run(task);
  }
}
