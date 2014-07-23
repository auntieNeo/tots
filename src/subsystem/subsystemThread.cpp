#include "subsystemThread.h"

#include "gameState.h"
#include "subsystem.h"
#include "threadPool.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  SubsystemThread::SubsystemThread(const char *name, const GameState *gameState) {
    m_currentSubsystem = NULL;

    // copy the game state from the given gameState
    m_gameState = new GameState(*gameState);

    // mark this thread as not free (it's not ready yet)
    SDL_AtomicSet(&m_free, 0);

    // create an SDL semaphore to control our thread
    m_runSemaphore = SDL_CreateSemaphore(0);

    // create our SDL thread
    m_sdlThread = SDL_CreateThread(getRunCallback(), name, this);
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
}
