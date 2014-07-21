#ifndef SUBSYSTEM_THREAD_H_
#define SUBSYSTEM_THREAD_H_

#include <SDL2/SDL_thread.h>

namespace tots {
  class GameState;
  class Subsystem;
  class ThreadPool;
  class SubsystemThread {
    friend class ThreadPool;
    public:
      SubsystemThread(int threadIndex, ThreadPool *pool, const GameState* gameState);
      ~SubsystemThread();

      void run(Subsystem *subsystem);

    protected:
      SDL_atomic_t m_free;

    private:
      GameState *m_gameState;
      Subsystem *m_currentSubsystem;

      SDL_Thread *m_sdlThread;
      SDL_sem *m_runSemaphore;

      ThreadPool *m_pool;
      int m_threadIndex;

      static int m_run(void *self);
  };
}

#endif
