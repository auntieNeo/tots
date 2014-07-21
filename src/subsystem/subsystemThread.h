#ifndef SUBSYSTEM_THREAD_H_
#define SUBSYSTEM_THREAD_H_

#include <SDL2/SDL_thread.h>

namespace tots {
  class GameState;
  class Subsystem;
  class SubsystemThread {
    public:
      SubsystemThread(int threadNumber, ThreadPool *pool, const GameState* gameState);
      ~SubsystemThread();

      void run(Subsystem *subsystem);

    private:
      GameState *m_gameState;
      Subsystem *m_currentSubsystem;

      SDL_Thread *m_sdlThread;

      ThreadPool *m_pool;

      static int m_run(void *self);
  };
}

#endif
