#include "subsystemThread.h"

#include "gameState.h"
#include "subsystem.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  SubsystemThread::SubsystemThread(const char *name) {
    m_gameState = new GameState();
    m_currentSubsystem = NULL;
    m_sdlThread = SDL_CreateThread(m_run, name, this);
  }

  SubsystemThread::~SubsystemThread() {
    delete m_gameState;
  }

  void SubsystemThread::run(Subsystem *subsystem) {
    // TODO: update the game state

    subsystem->update(m_gameState);
  }

  int SubsystemThread::m_run(void *voidSelf) {
    SubsystemThread *self = static_cast<SubsystemThread *>(voidSelf);
    // TODO: wait for semaphore, update subsystem, loop
    return 0;
  }
}
