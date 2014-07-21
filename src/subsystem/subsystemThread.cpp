#include "subsystemThread.h"

#include "gameState.h"
#include "subsystem.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  SubsystemThread::SubsystemThread(int threadNumber, ThreadPool *pool, const GameState *gameState) {
    m_currentSubsystem = NULL;
    m_pool = pool;

    m_gameState = new GameState(*gameState);

    char *threadName = new char[256];
    snprintf(threadName, 256, "Subsystem Thread %02ld", threadNumber);
    m_sdlThread = SDL_CreateThread(m_run, threadName, this);
    delete[] threadName;
  }

  SubsystemThread::~SubsystemThread() {
    // wait for the thread
    SDL_WaitThread(m_sdlThread, NULL);

    // delete our game state
    delete m_gameState;
  }

  void SubsystemThread::run(Subsystem *subsystem) {
    // TODO: assert that the thread is not running
    // TODO: run the subsystem in an SDL thread

    subsystem->update(m_gameState);
  }

  int SubsystemThread::m_run(void *voidSelf) {
    SubsystemThread *self = static_cast<SubsystemThread *>(voidSelf);
    // TODO: wait for semaphore, update subsystem, loop
    return 0;
  }
}
