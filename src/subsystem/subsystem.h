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
    friend class ThreadPool;
    public:
      enum Hints { NONE = 0, HOG_THREAD = 2 };

      Subsystem();
      virtual ~Subsystem();

      void init(const GameState *state);
      void update(const GameState *state);
      // TODO: add close() member function
      virtual Hints hints() const { return NONE; };

    protected:
      virtual void m_init(const GameState *state) = 0;
      virtual void m_update(const GameState *state) = 0;
      size_t m_dt;

    private:
      EventQueue *m_input, *m_output;
      SubsystemThread *m_lastThread, *m_hoggedThread;
  };
}

#endif
