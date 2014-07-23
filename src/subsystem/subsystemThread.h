#ifndef SUBSYSTEM_THREAD_H_
#define SUBSYSTEM_THREAD_H_

#include "../common.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  class GameState;
  class Subsystem;
  class SubsystemThread {
    public:
      enum Command { INIT = 1, UPDATE, CLOSE };

      SubsystemThread(const char *name, const GameState *gameState);
      ~SubsystemThread();

      virtual void run(Subsystem *subsystem, SubsystemThread::Command command) = 0;

      bool isFree() { return SDL_AtomicGet(&m_free); }

    protected:
      void setFree(bool free) { SDL_AtomicSet(&m_free, free); }

      Subsystem *currentSubsystem() const { return m_currentSubsystem; }

      void waitRun() { SDL_SemWait(m_runSemaphore); }
      void signalRun() { assert(SDL_SemValue(m_runSemaphore) == 0);
                         SDL_SemPost(m_runSemaphore); }

      void runCurrentSubsystem(Command command) {
        switch(command) {
          case INIT:
            m_currentSubsystem->init(m_gameState);
            break;
          case UPDATE:
            m_currentSubsystem->update(m_gameState);
            break;
          case CLOSE:
            m_currentSubsystem->close(m_gameState);
            break;
          default:
            assert(false);
        }
      }

    private:
      GameState *m_gameState;
      Subsystem *m_currentSubsystem;

      SDL_Thread *m_sdlThread;
      SDL_sem *m_runSemaphore;
      SDL_atomic_t m_free;

      void setCurrentSubsystem(Subsystem *subsystem) { m_currentSubsystem = subsystem; }

      virtual SDL_ThreadFunction getRunCallback() const = 0;

      static int m_run(void *self);
  };
}

#endif
