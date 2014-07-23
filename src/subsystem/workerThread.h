#ifndef WORKER_THREAD_H_
#define WORKER_THREAD_H_

#include <SDL2/SDL_thread.h>

namespace tots {
  class ThreadPool;
  class WorkerThread : public SubsystemThread {
    friend class ThreadPool;
    public:
      WorkerThread(int threadIndex, ThreadPool *pool, const GameState *gameState);
      ~WorkerThread();

      void run(Subsystem *subsystem, SubsystemThread::Command command);

    private:

      ThreadPool *m_pool;
      int m_threadIndex;

      SDL_ThreadFunction getRunCallback() const { return m_run; }

      static int m_run(void *self);
  };
}

#endif
