#ifndef SUBSYSTEM_H_
#define SUBSYSTEM_H_

#include "../common.h"

namespace tots { namespace framework {
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
    friend class ThreadPool;
    friend class GameLoop;
    friend class Task;  // FIXME: should not need this; should be SubsystemTask
    public:
      /**
       * Subsystem::Command enumerates all of the abstract virtual methods that
       * the Subsystem class exposes to its descendants. This facilitates the
       * scheduling of Subsystem + Subsystem::Command pairs in the GameLoop as
       * well as the method resolution in
       * SubsystemThread::runCurrentSubsystem().
       */
      enum class Command {
        /// Corresponds to the Subsystem::init() method.
        INIT = 1,
        /// Corresponds to the Subsystem::update() method.
        UPDATE,
        /// Corresponds to the Subsystem::close() method.
        CLOSE
      };

      /**
       * Subsystem::Hints enumerates some bitwise flags for passing runtime
       * hints to the GameLoop class and possibly other classes that interact
       * with the Subsystem. With the exception of Hints::NONE, flags can be
       * combined with the usual bitwise OR operator.
       *
       * If derived Subsystem classes want or need to take advantage of these
       * hints, they should implement the virtual hints() method. Short
       * descriptions of the behavior that each hint enables are given below.
       * Longer descriptions may or may not be found in the documentation for
       * the classes that utilize these hints.
       *
       * \sa hints()
       */
      enum class Hints : uint32_t {
        /**
         * This value represents no subsystem hints enabled. This is the
         * default for Subsystem classes that don't implement the
         * Subsystem::hints() method.
         */
        NONE = 0x00,
        /**
         * With the HOG_THREAD hint enabled, a Subsystem will be given its
         * own thread that is never shared. This is useful when using third
         * party libraries such as most OpenGL implementations that don't
         * support being accessed from multiple threads.
         */
        HOG_THREAD = 0x02,
        /**
         * With the UPDATE_EACH_FRAME hint enabled, the GameLoop object's task
         * scheduler will always schedule the Subsystem object to update at
         * the beginning of the rendering frame, or as close to the beginning
         * as possible.
         */
        UPDATE_EACH_FRAME = 0x04
      };

      /**
       * Subsystem::Priority enumerates the possible priority values used when
       * scheduling a Subsystem object for execution, as well as some special
       * priority values that have special interpretations in the GameLoop
       * class' scheduling routines.
       *
       * Because the GameLoop class' priority queue is implemented as a
       * MinPriorityQueue, lower enum values correspond with higher priority.
       */
      enum class Priority : uint32_t {
        /// The highest priority, and the lowest enum value possible.
        HIGHEST = 0,
        /// Higher priority than NORMAL. Not the highest priority.
        HIGH = 8,
        /// Normal priority; the default priority.
        NORMAL = 16,
        /// Lower priority than NORMAL. Not the lowest priority.
        LOW = 24,
        /**
         * The lowest priority possible. Any enum values lower than this are
         * either special or invalid values.
         */
        LOWEST = 32,
        /**
         * This enum value is used by the GameLoop class' scheduling routine
         * to schedule a Subsystem object with whatever priority that Subsystem
         * advertises through the Subsystem::priority() method.
         */
        SUBSYSTEM = 42
      };

      Subsystem();
      virtual ~Subsystem();

      void init(const GameState *state);
      void update(const GameState *state);
      void close(const GameState *state);

      /**
       * Derived Subsystem classes can implement the hints() method to return a
       * Subsystem::Hints flag. Multiple flags can be combined by using a
       * bitwise OR operation in the usual fashion.
       *
       * These flags affect how the GameLoop and other objects that interact
       * with the Subsystem object treat a particular Subsystem object.
       *
       * See the documentation for the Subsystem::Hints enumeration for a
       * description of the possible flag values and what they do.
       */
      virtual Hints hints() const { return Hints::NONE; };

      /**
       * Derived Subsystem classes must implement the name() method to return a
       * suitable name for the subsystem, for debugging purposes.
       */
      virtual const char *name() const = 0;

      /**
       * The updatePeriod() returns an integer indicating how often the
       * GameLoop object's scheduler should call update() on the Subsystem
       * object.
       *
       * If updatePeriod() returns a positive integer \i n, then the scheduler
       * will schedule an update once every \i n game ticks.
       *
       * If updatePeriod() returns zero, the scheduler assumes that the update
       * period should be determined by one or more hint flag provided by the
       * hints() method.
       *
       * if updatePeriod() returns a negative number, the scheduler will not
       * schedule updates for the subsystem.
       *
       * Derived Subsystem classes must implement the updatePeriod() method.
       *
       * \sa Hints::EACH_FRAME
       */
      virtual int32_t updatePeriod() const = 0;

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

      /**
       * Derived classes must implement the m_update() method. m_update() is
       * called regularly from the GameLoop according to the Subsystem object's
       * period. Other factors, such as thread availability, can affect how
       * often m_update() is called.
       *
       * Implement this method to increment the derived Subsystem object's
       * simulation of the game world for each subsystem tick.
       *
       * \sa update()
       */
      virtual void m_update(const GameState *state) = 0;

      /**
       * Dervied classes msut implement the m_close() method. m_close() is
       * called late in the life of the GameLoop object and is intended to be
       * used for cleaning up any resources allocated in the Subsystem's
       * lifetime.
       *
       * \sa close()
       */
      virtual void m_close(const GameState *state) = 0;

//      size_t m_dt;  // TODO: expose a delta time somewhere

    private:
      EventQueue *m_input, *m_output;
      SubsystemThread *m_lastThread, *m_hoggedThread;

      void setLastThread(SubsystemThread *lastThread) { m_lastThread = lastThread; }
      SubsystemThread *lastThread() const { return m_lastThread; }

      void setHoggedThread(SubsystemThread *hoggedThread) { m_hoggedThread = hoggedThread; }
      SubsystemThread *hoggedThread() const { return m_hoggedThread; }
  };

  constexpr Subsystem::Hints
  operator&(Subsystem::Hints a, Subsystem::Hints b) {
    return static_cast<Subsystem::Hints>(
        static_cast<uint32_t>(a) & static_cast<uint32_t>(b)
        );
  }

  constexpr Subsystem::Hints
  operator|(Subsystem::Hints a, Subsystem::Hints b) {
    return static_cast<Subsystem::Hints>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
        );
  }
} }

#endif
