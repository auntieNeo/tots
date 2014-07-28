#ifndef HOGGED_THREAD_H_
#define HOGGED_THREAD_H_

#include <SDL_thread.h>

#include "subsystemThread.h"

namespace tots {
  class GameState;
  class Subsystem;
  class HoggedThread : public SubsystemThread {
    public:
      HoggedThread(const char *name, const GameState *gameState);
      ~HoggedThread();

      void run(Subsystem *subsystem, SubsystemThread::Command command);

      void waitThreadSemaphore() { SDL_SemWait(m_threadSemaphore); }

    private:
      SubsystemThread::Command m_command;
      SDL_sem *m_threadSemaphore;

      virtual SDL_ThreadFunction getRunCallback() const { return m_run; };
      static int m_run(void *self);

      void postThreadSemaphore() { SDL_SemPost(m_threadSemaphore); }
  };
}

#endif
