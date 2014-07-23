#ifndef HOGGED_THREAD_H_
#define HOGGED_THREAD_H_

namespace tots {
  class HoggedThread : public SDLThread {
    public:
      HoggedThread(const char *name, const GameState *gameState);
      ~HoggedThread();

      void run(Subsystem *subsystem, SubsystemThread::Command command);
  };
}

#endif
