#ifndef SUBSYSTEM_H_
#define SUBSYSTEM_H_

#include "../common.h"

namespace tots {
  class GameState;
  class EventQueue;
  class ThreadPool;
  class SubsystemThread;

  /**
   * The Subsystem class embodies a system in the Tots engine. Derived classes
   * can be used to extend the functionality of the Tots engine. Instantiated
   * Subsystem objects are passed to the GameLoop class when starting the
   * engine to use them.
   *
   * Subsystems can be run in multiple threads, and they must communicate using
   * Message queues. This design serves to decouple different systems in the
   * Tots engine, and also facilitates message passing among subsystem threads
   * without excessive thread locking.
   */
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
      /**
       * Derived classes must implement the m_init() method. m_init() is called
       * early from the GameLoop object to allow Subsystem objects to initialize
       * their resources in a thread.
       *
       * If some resource must be initialized from a thread other than the main
       * thread, or if a subsystem's performance would benefit from doing so,
       * those resources should be initialized inside m_init(). Resources that
       * would be better initialized from the main thread can be initialized in
       * the derived subsystem's constructor.
       *
       * \sa init()
       */
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
