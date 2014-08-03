#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include "../common.h"
#include "../utility/priorityQueue.h"
#include "subsystem.h"

namespace tots {
  class GameState;
  class AggregateQueue;
  class ThreadPool;
  class Task;
  class TaskQueue;

  /**
   * The GameLoop class represents the outermost loop of the Tots engine. The
   * GameLoop's primary task is to dispatch the various subsystems on one or
   * more threads.
   *
   * GameLoop implements a non-locking game loop that shares data between
   * subsystem threads using an aggregate command queue and multiple copies of
   * the GameState object.
   *
   * Games that use the Tots engine must construct a GameLoop object with a
   * list of Subsystem objects that will be used for the duration of the
   * GameLoop. The game can set different parameters on the GameLoop to affect
   * its behavior, such as the framerate, the number of frames to skip, and the
   * period of the game clock.
   *
   * Additionally, through the Subsystem class interface, each Subsystem object
   * advertises the period on which it must be scheduled for updates, as well as
   * hint flags that affect the GameLoop's behavior. Descriptions of these
   * attributes and flags are described in the Subsystem class' API.
   *
   * The operation of the GameLoop is subtle and complicated. See these
   * resources:
   *
   * <a href="http://gameprogrammingpatterns.com/game-loop.html">Game Loop - Game Programming Patterns</a>
   *
   * <a href="http://gafferongames.com/game-physics/fix-your-timestep/">Fix Your Timestep! - gafferongames.com</a>
   */
  class GameLoop {
    public:
      GameLoop(Subsystem **subsystems, size_t numSubsystems);
      ~GameLoop();

      void run();

      // game loop parameters
      /**
       * A cap in milliseconds on how long the main loop is allowed to hang.
       * This prevents the main loop from entering a "spiral of death" when
       * whatever task it is performing goes over its time bugdet and falls
       * into a state of spiraling time debt.
       *
       * \sa setLoopHangCap()
       */
      uint32_t loopHangCap() const { return m_loopHangCap; }
      /**
       * A cap in milliseconds on how long the main loop is allowed to hang.
       * \sa loopHangCap()
       */
       void setLoopHangCap(uint32_t loopHangCap) { m_loopHangCap = loopHangCap; }

       /**
        * The number of frames to render in each second. Internally, this is
        * represented by the number of milliseconds between frames.
        *
        * \sa setFramesPerSecond()
        */
       uint32_t framesPerSecond() const { return m_framesPerSecond; }
       /**
        * The number of frames to render in each second. Internally, this is
        * represented by the number of milliseconds between frames.
        *
        * \sa framesPerSecond()
        */
       void setFramesPerSecond(uint32_t framesPerSecond) {
         m_framesPerSecond = framesPerSecond;
         m_frameTime = 1 / (m_framesPerSecond/1000.0f);
       }

    private:
      GameState *m_state;
//      AggregateQueue<Message> *m_messageQueue;
      TaskQueue *m_taskQueue, *m_overdueTaskQueue;

      Subsystem **m_subsystems;
      size_t m_numSubsystems;

      ThreadPool *m_threads;

      // game loop parameters
      uint32_t m_loopHangCap;
      uint32_t m_framesPerSecond;
      double m_frameTime; // milliseconds per frame
//      uint32_t m_gameTick = 1;

      void m_scheduleTask(const Task &task, uint32_t gameTime, Subsystem::Priority priority, TaskQueue *queue);
      void m_scheduleTask(const Task &task, uint32_t gameTime, Subsystem::Priority priority) { m_scheduleTask(task, gameTime, priority, m_taskQueue); }
      bool m_tryRunTask(Task &task);
  };
}

#endif
