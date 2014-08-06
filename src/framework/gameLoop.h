#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

#include "../common.h"
#include "../utility/priorityQueue.h"
#include "subsystem.h"

namespace tots { namespace framework {
  class GameState;
  class AggregateQueue;
  class ThreadPool;
  class ThreadSignal;
  class Task;
  class TaskQueue;

  /**
   * The GameLoop class represents the outermost loop of the Tots engine. The
   * GameLoop's primary task is to dispatch the various subsystems on one or
   * more threads.
   *
   * GameLoop implements a non-locking game loop that shares data between
   * subsystem threads using an aggregate message queue and multiple copies of
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
   *
   * The following sequence diagram is an example of possible deterministic
   * operation of the game loop. It is just an example and still a work in
   * progress.
   *
   * @startuml{gameLoopExample.png}
   * !include ../../documentation/plantuml/doxygenColors.iuml
   * participant GameLoop
   * participant Graphics
   * participant Physics
   * participant Collision
   * participant Resources
   * participant Audio
   *
   * == Initialization (t = 0) ==
   *
   * autonumber 0 0 "t = ##"
   *
   * GameLoop ->> Graphics: <b>INIT</b>
   * activate Graphics
   * GameLoop ->> Physics: <b>INIT</b>
   * activate Physics
   * GameLoop ->> Collision: <b>INIT</b>
   * activate Collision
   * GameLoop ->> Resources: <b>INIT</b>
   * activate Resources
   * GameLoop ->> Audio: <b>INIT</b>
   * activate Audio
   *
   * GameLoop <<- Graphics
   * deactivate Graphics
   * GameLoop <<- Physics
   * deactivate Physics
   * GameLoop <<- Collision
   * deactivate Collision
   * GameLoop <<- Resources
   * deactivate Resources
   * GameLoop <<- Audio
   * deactivate Audio
   *
   * == Frame 0 (t = 0) ==
   *
   * autonumber 0 0 "t = ##"
   *
   * note over Graphics : Graphics
   * GameLoop -->> Graphics: <b>UPDATE</b>
   * activate Graphics
   * note over Physics : Physics
   * GameLoop ->> Physics: <b>UPDATE</b>
   * activate Physics
   * note over Resources : Resources
   * GameLoop ->> Resources: <b>UPDATE</b>
   * activate Resources
   * note over Audio : Audio
   * GameLoop ->> Audio: <b>UPDATE</b>
   * activate Audio
   *
   * autonumber 7 0 "t = ##"
   *
   * GameLoop <<-- Audio
   * deactivate Audio
   *
   * autonumber 16 0 "t = ##"
   *
   * GameLoop <<-- Graphics
   * deactivate Graphics
   *
   * == Frame 1 (t = 16) ==
   *
   * autonumber 16 0 "t = ##"
   *
   * note over Graphics : Graphics
   * GameLoop -->> Graphics: <b>UPDATE</b>
   * activate Graphics
   *
   * note over Collision : Collision
   * GameLoop ->> Collision: <b>UPDATE</b>
   * activate Collision
   *
   * autonumber 32 0 "t = ##"
   *
   * GameLoop <<-- Graphics
   * deactivate Graphics
   *
   * note over Physics : Physics
   * GameLoop <<- Physics
   * deactivate Physics
   *
   * == Frame 2 (t = 32) ==
   *
   * autonumber 32 0 "t = ##"
   *
   * note over Physics : Physics
   * GameLoop ->> Physics: <b>UPDATE</b>
   * activate Physics
   * 
   * autonumber 48 0 "t = ##"
   *
   * note over Collision : Collision
   * GameLoop <<- Collision
   * deactivate Collision
   *
   * == Frame 3 (t = 48) ==
   *
   * autonumber 48 0 "t = ##"
   *
   * note over Collision : Collision
   * GameLoop ->> Collision: <b>UPDATE</b>
   * activate Collision
   *
   * autonumber 64 0 "t = ##"
   *
   * note over Physics : Physics
   * GameLoop <<- Physics
   * deactivate Physics
   *
   * == Frame 4 (t = 64) ==
   *
   * autonumber 64 0 "t = ##"
   *
   * note over Physics : Physics
   * GameLoop ->> Physics
   * activate Physics
   *
   * autonumber 80 0 "t = ##"
   *
   * note over Collision : Collision
   * GameLoop <<- Collision
   * deactivate Collision
   *
   * == Frame 5 (t = 80) ==
   *
   * autonumber 80 0 "t = ##"
   *
   * note over Collision : Collision
   * GameLoop ->> Collision: <b>UPDATE</b>
   * activate Collision
   *
   * autonumber 96 0 "t = ##"
   *
   * note over Physics : Physics
   * GameLoop <<- Physics
   * deactivate Physics
   *
   * == Frame 6 (t = 96) ==
   *
   * autonumber 96 0 "t = ##"
   *
   * note over Physics : Physics
   * GameLoop ->> Physics: <b>UPDATE</b>
   * activate Physics
   *
   * autonumber 100 0 "t = ##"
   *
   * note over Resources : Resources
   * GameLoop <<- Resources
   * deactivate Resources
   *
   * autonumber 112 0 "t = ##"
   *
   * note over Collision : Collision
   * GameLoop <<- Collision
   * deactivate Collision
   *
   * deactivate Physics
   *
   * ... Many Frames Later ...
   *
   * == Closing (t = n) ==
   *
   * ' it is impossible to disable autonumber <_<
   * autonumber 0 0 " "
   *
   * GameLoop ->> Graphics: t = n <b>CLOSE</b>
   * activate Graphics
   * GameLoop ->> Physics: t = n <b>CLOSE</b>
   * activate Physics
   * GameLoop ->> Collision: t = n <b>CLOSE</b>
   * activate Collision
   * GameLoop ->> Resources: t = n <b>CLOSE</b>
   * activate Resources
   * GameLoop ->> Audio: t = n <b>CLOSE</b>
   * activate Audio
   *
   * GameLoop <<- Graphics: t = n
   * deactivate Graphics
   * GameLoop <<- Physics: t = n
   * deactivate Physics
   * GameLoop <<- Collision: t = n
   * deactivate Collision
   * GameLoop <<- Resources: t = n
   * deactivate Resources
   * GameLoop <<- Audio: t = n
   * deactivate Audio
   *
   * @enduml
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
      TaskQueue *m_taskQueue, *m_pendingTaskQueue;

      Subsystem **m_subsystems;
      size_t m_numSubsystems;

      // thread resources
      ThreadPool *m_threads;
      ThreadSignal *m_signal;

      // game loop parameters
      uint32_t m_loopHangCap;
      uint32_t m_framesPerSecond;
      double m_frameTime; // milliseconds per frame
//      uint32_t m_gameTick = 1;

      void m_scheduleTask(const Task &task, uint32_t gameTime, Subsystem::Priority priority, TaskQueue *queue);
      void m_scheduleTask(const Task &task, uint32_t gameTime, Subsystem::Priority priority) { m_scheduleTask(task, gameTime, priority, m_taskQueue); }
      bool m_tryRunTask(Task &task);
  };
} }

#endif
