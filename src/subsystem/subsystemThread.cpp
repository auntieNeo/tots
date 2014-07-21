#include "subsystemThread.h"

#include "gameState.h"
#include "subsystem.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  SubsystemThread::SubsystemThread(int threadIndex, ThreadPool *pool, const GameState *gameState) {
    m_currentSubsystem = NULL;
    m_pool = pool;
    m_threadIndex = threadIndex;

    // copy the game state from the given gameState
    m_gameState = new GameState(*gameState);

    // create an SDL semaphore to control our thread
    m_threadSemaphore = SDL_CreateSemaphore(0);

    // create our SDL thread
    char *threadName = new char[256];
    snprintf(threadName, 256, "Subsystem Thread %02d", threadIndex);
    m_sdlThread = SDL_CreateThread(m_run, threadName, this);
    delete[] threadName;
  }

  SubsystemThread::~SubsystemThread() {
    assert(SDL_AtomicGet(&(m_pool->m_done)));
    // wait for our thread
    SDL_WaitThread(m_sdlThread, NULL);

    // destroy our semaphore
    SDL_DestroySemaphore(m_threadSemaphore);

    // delete our game state
    delete m_gameState;
  }

  void SubsystemThread::run(Subsystem *subsystem) {
    // assert that this thread is free and not running
    assert(m_pool->m_freeThreads & (1 << m_threadIndex));

    // change the current subsystem
    m_currentSubsystem = subsystem;

    // mark this thread as not free in m_pool->m_freeThreads
    SDL_AtomicSet(&m_free, 0);

    // signal the thread to start running
    SDL_SemPost(m_runSemaphore);
  }

  int SubsystemThread::m_run(void *voidSelf) {
    SubsystemThread *self = static_cast<SubsystemThread *>(voidSelf);
    while(!SDL_AtomicGet(&(m_pool->m_done))) {
      // mark this thread as free
      SDL_AtomicSet(&m_free, 1);

      // post to m_pool->m_threadSemaphore to let main thread know we're available
      SDL_SemPost(m_pool->m_threadSemaphore);

      // wait for m_runSemaphore signal (main thread wants us to run now)
      SDL_SemWait(m_runSemaphore);

      // TODO: automatically update the game state

      // run the subsystem
      self->m_currentSubsystem->update(m_gameState);
    }
    return 0;
  }
}
