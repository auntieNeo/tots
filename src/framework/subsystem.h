#ifndef SUBSYSTEM_H_
#define SUBSYSTEM_H_

#include "../common.h"

namespace tots {
  class GameState;
  class EventQueue;
  class ThreadPool;
  class SubsystemThread;
  class Subsystem {
    friend class SubsystemThread;
    friend class WorkerThread;
    friend class ThreadPool;
    public:
      enum Command { INIT = 1, UPDATE, CLOSE };
      enum Hints { NONE = 0, HOG_THREAD = 2 };
      enum Priority { HIGHEST_PRIORITY = 0, HIGH_PRIORITY = 8, NORMAL_PRIORITY = 16, LOW_PRIORITY = 24, LOWEST_PRIORITY = 32, SUBSYSTEM_PRIORITY = 42 };

      Subsystem();
      virtual ~Subsystem();

      void init(const GameState *state);
      void update(const GameState *state);
      void close(const GameState *state);
      virtual Hints hints() const { return NONE; };

      virtual const char *name() const = 0;

    protected:
      virtual void m_init(const GameState *state) = 0;
      virtual void m_update(const GameState *state) = 0;
      virtual void m_close(const GameState *state) = 0;
      size_t m_dt;

    private:
      EventQueue *m_input, *m_output;
      SubsystemThread *m_lastThread;

      void setLastThread(SubsystemThread *lastThread) { m_lastThread = lastThread; }
      SubsystemThread *lastThread() const { return m_lastThread; }
  };
}

#endif
