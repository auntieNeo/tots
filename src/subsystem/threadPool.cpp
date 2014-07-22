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
    m_threads = new SubsystemThread*[MAX_THREADS];
    m_hoggedThreads = new SubsystemThread*[MAX_THREADS];
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
    for(size_t i = 0; i < m_numHoggedThreads; ++i) {
      delete m_hoggedThreads[i];
    }
    SDL_DestroySemaphore(m_threadSemaphore);
    delete[] m_threads;
    delete[] m_hoggedThreads;
  }

  // FIXME: duplicated code here
  void ThreadPool::init(Subsystem *subsystem) {
    // acquire a thread for a hog
    if(subsystem->hints() & Subsystem::HOG_THREAD) {
      assert(m_numThreads >= 2);  // FIXME: come up with a strategy for increasing the number of threads when this assertion fails
      assert(subsystem->m_hoggedThread == NULL);
      
      subsystem->m_hoggedThread = m_threads[--m_numThreads];
      m_hoggedThreads[m_numHoggedThreads++] = subsystem->m_hoggedThread;

      // FIXME: We don't even wait for the thread or decrement m_threadSemaphore here. This is obviously broken. Need to refactor ASAP.
      // initialize the subsystem on the hogged thread
      subsystem->m_hoggedThread->run_init(subsystem);
      return;
    }

    // wait for a free thread
    SDL_SemWait(m_threadSemaphore);

    // find the first free thread
    int threadIndex = -1;
    for(size_t i = 0; i < m_numThreads; ++i) {
      if(SDL_AtomicGet(&(m_threads[i]->m_free))) {
        threadIndex = i;
        break;
      }
    }
    assert(threadIndex != -1);

    // initialize the subsystem on that thread
    m_threads[threadIndex]->run_init(subsystem);
  }

  // FIXME: duplicated code here
  void ThreadPool::run(Subsystem *subsystem) {
    // run with a hogged thread
    if(subsystem->m_hoggedThread != NULL) {
      assert(SDL_AtomicGet(&(subsystem->m_hoggedThread->m_free)));
      subsystem->m_hoggedThread->run(subsystem);
      return;
    }

    // wait for a free thread
    SDL_SemWait(m_threadSemaphore);

    // try using the same thread, to avoid cache misses
    // FIXME: assert to prevent subsystems from running on hogged threads in this way
    if(subsystem->m_lastThread != NULL && SDL_AtomicGet(&(subsystem->m_lastThread->m_free))) {
      subsystem->m_lastThread->run(subsystem);
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
    m_threads[threadIndex]->run(subsystem);
  }
}
