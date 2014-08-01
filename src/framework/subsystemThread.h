#ifndef SUBSYSTEM_THREAD_H_
#define SUBSYSTEM_THREAD_H_

#include "../common.h"
#include "subsystem.h"

#include <SDL_thread.h>

namespace tots {
  class GameState;

  /**
   * The SubsystemThread class encapsulates an SDL thread that runs a Subsystem
   * object.
   *
   * The SubsystemThread is also responsible for executing the upkeep for the
   * GameState object. Every time before a new subsystem is executed, the
   * GameState object is updated from events in the message queue that modify
   * the game state. By maintaining a seperate GameState object inside each
   * SubsystemThread object, the Tots engine avoids a large amount of thread
   * synchronization. Because the otherwise serial synchronization step of
   * updating the GameState is run in every thread, its cost is effectively
   * reduced to the cost of running it once.
   */
  class SubsystemThread {
    friend class ThreadPool;
    public:
      SubsystemThread(const char *name, const GameState *gameState, SDL_sem *readySemaphore);
      virtual ~SubsystemThread();
      void init(const char *name);

      void run(Subsystem *subsystem, Subsystem::Command command);

      bool isFree() { return SDL_AtomicGet(&m_free); }

    protected:
      void setFree(bool free) { SDL_AtomicSet(&m_free, free); }

      bool isDone() { return SDL_AtomicGet(&m_done); }
      void setDone(bool done) { SDL_AtomicSet(&m_done, done); }

      Subsystem *currentSubsystem() const { return m_currentSubsystem; }
      void setCurrentSubsystem(Subsystem *subsystem) { m_currentSubsystem = subsystem; }

      void waitRun() { SDL_SemWait(m_runSemaphore); }
      void signalRun() { assert(SDL_SemValue(m_runSemaphore) == 0);
                         SDL_SemPost(m_runSemaphore); }

      void runCurrentSubsystem(Subsystem::Command command) {
        switch(command) {
          case Subsystem::INIT:
            m_currentSubsystem->init(m_gameState);
            break;
          case Subsystem::UPDATE:
            m_currentSubsystem->update(m_gameState);
            break;
          case Subsystem::CLOSE:
            m_currentSubsystem->close(m_gameState);
            break;
          default:
            assert(false);
        }
      }

    private:
      GameState *m_gameState;
      Subsystem *m_currentSubsystem;
      Subsystem::Command m_command;

      SDL_Thread *m_sdlThread;
      SDL_sem *m_runSemaphore, *m_readySemaphore;
      SDL_atomic_t m_free, m_done;

      void signalReady() { SDL_SemPost(m_readySemaphore); };

      static int m_run(void *self);
  };
}

#endif
