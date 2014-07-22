#include "subsystemThread.h"

#include "gameState.h"
#include "subsystem.h"
#include "threadPool.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  SubsystemThread::SubsystemThread(int threadIndex, ThreadPool *pool, const GameState *gameState) {
    m_currentSubsystem = NULL;
    m_pool = pool;

    // copy the game state from the given gameState
    m_gameState = new GameState(*gameState);

    // mark this thread as not free
    SDL_AtomicSet(&m_free, 0);

    // create an SDL semaphore to control our thread
    m_runSemaphore = SDL_CreateSemaphore(0);

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
    SDL_DestroySemaphore(m_runSemaphore);

    // delete our game state
    delete m_gameState;
  }

  // FIXME: duplicated code here
  void SubsystemThread::run(Subsystem *subsystem) {
    // assert that this thread is free and not running
    assert(SDL_AtomicGet(&m_free));

    // change the current subsystem
    m_currentSubsystem = subsystem;
    m_init = false;

    // set the last thread for this subsystem
    m_currentSubsystem->m_lastThread = this;

    // mark this thread as not free
    SDL_AtomicSet(&m_free, 0);

    // signal the thread to start running
    SDL_SemPost(m_runSemaphore);
  }

  // FIXME: duplicated code here
  void SubsystemThread::run_init(Subsystem *subsystem) {
    // assert that this thread is free and not running
    assert(SDL_AtomicGet(&m_free));

    // change the current subsystem
    m_currentSubsystem = subsystem;
    m_init = true;

    // NOTE: _don't_ set the last thread for this subsystem, as this can hurt thread hogs. We accept the possible cache miss, at least until I can find a better solution.
    //m_currentSubsystem->m_lastThread = this;

    // mark this thread as not free
    SDL_AtomicSet(&m_free, 0);

    // signal the thread to start running
    SDL_SemPost(m_runSemaphore);
  }

  int SubsystemThread::m_run(void *voidSelf) {
    SubsystemThread *self = static_cast<SubsystemThread *>(voidSelf);
    while(!SDL_AtomicGet(&(self->m_pool->m_done))) {
      // mark this thread as free
      SDL_AtomicSet(&(self->m_free), 1);

      // post to m_pool->m_threadSemaphore to let main thread know we're available
      // FIXME: This is probably broken with thread hogging. Need to refactor this code ASAP.
      if(self->m_currentSubsystem == NULL || !(self->m_currentSubsystem->hints() & Subsystem::HOG_THREAD))
        SDL_SemPost(self->m_pool->m_threadSemaphore);

      // wait for m_runSemaphore signal (main thread wants us to run now)
      SDL_SemWait(self->m_runSemaphore);

      // TODO: automatically update the game state

      // run the subsystem
      if(self->m_init)
        self->m_currentSubsystem->init(self->m_gameState);
      else
        self->m_currentSubsystem->update(self->m_gameState);
    }
    return 0;
  }
}
