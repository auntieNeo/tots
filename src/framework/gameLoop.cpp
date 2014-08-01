#include "gameLoop.h"

#include "gameState.h"
#include "threadPool.h"

#define NUM_THREADS 1

namespace tots {
  /**
   * The GameLoop class represents the outermost loop of the Tots engine. The
   * GameLoop's primary task is to dispatch the various subsystems on one or
   * more threads.
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
  GameLoop::GameLoop(Subsystem **subsystems, size_t numSubsystems) {
    // create an empty game state to share among the threads
    m_state = new GameState();  // FIXME: we might not even need to keep this around...

    // create the master message queue
//    m_messageQueue = new AggregateQueue();

    // create the task queue which schedules subsystems to run
    m_taskQueue = new TaskQueue(numSubsystems * 4);

    // TODO: populate the game state

    // create a pool of threads
    m_threads = new ThreadPool(NUM_THREADS, m_state);

    // copy the subsystem pointers
    m_numSubsystems = numSubsystems;
    m_subsystems = new Subsystem*[numSubsystems];
    memcpy(m_subsystems, subsystems, sizeof(Subsystem *) * numSubsystems);

    // register each subsystem
    m_threads->registerSubsystems(m_subsystems, m_numSubsystems);

    // initialize each subsystem
    for(size_t i = 0; i < m_numSubsystems; ++i) {
      m_scheduleTask(m_subsystems[i], Subsystem::INIT, 0, Subsystem::HIGHEST_PRIORITY);
      m_threads->run(m_subsystems[i], Subsystem::INIT);
    }
  }

  GameLoop::~GameLoop() {
    delete m_threads;
    delete m_taskQueue;
//    delete m_messageQueue;
    delete m_state;
    delete[] m_subsystems;
  }

  void GameLoop::run() {
    /* TODO:
     * Combine message queues into one master message queue.
     * Execute master command queue on each thread's GameState.
     */

      // TODO: possibly introduce determinism by implementing subsystem priority and a thread gate

    while(1) {
      // if any tasks in the overdue queue
        // try to run overdue tasks

      // take a task from the task queue
      // check if the task can be run (i.e. threads are available)

      // if the task can't be scheduled now, add it to an overdue queue

      // FIXME: Need to actually wait for subsystems to be finished. This only hasn't crashed because drawing one triangle is fast. <_<
      for(size_t i = 0; i < m_numSubsystems; ++i) {
        m_threads->run(m_subsystems[i], Subsystem::UPDATE);  // FIXME: don't actually schedule subsystems like this
      }

      while(false) {
        // TODO: append GameState message queue to m_queue
        // m_queue->append(thread->gameState()->popMessageQueue());
        // TODO: append subsystem message queue to m_queue
        // m_queue->append(thread->currentSubsystem()->popMessageQueue());
      }

      // TODO: wait for each frame
      // TODO: drop frames
      // TODO: timeout and /stop/ dropping frames
    }

    // close all of the subsystems
    for(size_t i = 0; i < m_numSubsystems; ++i) {
      m_threads->run(m_subsystems[i], Subsystem::CLOSE);
    }
  }

  /**
   * The m_scheduleTask routine schedules \a task (which is a Subsystem pointer
   * and a command) to be run in a thread at the given time \a gameTime with the
   * priority \a priority.
   * \var Task task the var
   */
  void GameLoop::m_scheduleTask(Task task, uint32_t gameTime, Subsystem::Priority priority) {
    assert(sizeof(Subsystem::Priority) == 4);

    uint64_t key = (static_cast<uint64_t>(gameTime) << 32) | static_cast<uint64_t>(priority);
    m_taskQueue->insert(key, task);
  }
}
