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

    private:
      SubsystemThread::Command m_command;

      virtual SDL_ThreadFunction getRunCallback() const { return m_run; };
      static int m_run(void *self);
  };
}

#endif
