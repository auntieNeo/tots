#include "subsystemThread.h"

#include "gameState.h"
#include "subsystem.h"
#include "threadPool.h"

#include <SDL_thread.h>

namespace tots {
  SubsystemThread::SubsystemThread(const GameState *gameState) {
    m_currentSubsystem = NULL;

    // copy the game state from the given gameState
    m_gameState = new GameState(*gameState);

    // mark this thread as not free and not done (it's not even ready yet)
    setFree(false);
    setDone(false);

    // create an SDL semaphore to control our thread
    m_runSemaphore = SDL_CreateSemaphore(0);
  }

  void SubsystemThread::init(const char *name) {
    // create our SDL thread
    // FIXME: this line needs to be executed after the object has been constructed... there must be a better way
    m_sdlThread = SDL_CreateThread(getRunCallback(), name, this);
  }

  SubsystemThread::~SubsystemThread() {
    // mark as done and wait for our thread
    setDone(true);
    SDL_WaitThread(m_sdlThread, NULL);

    // destroy our semaphore
    SDL_DestroySemaphore(m_runSemaphore);

    // delete our game state
    delete m_gameState;
  }
}
