#include "gameLoop.h"

#include "gameState.h"
#include "threadPool.h"

#define NUM_THREADS 1

namespace tots {
  /**
   * The GameLoop constructor takes a list of Subsystem objects pointed to in
   * \a subsystems. These Subsystem objects will be used for the duration of the
   * GameLoop object. Subsystems cannot be added on the fly; they must be added
   * at the GameLoop object's construction.
   *
   * \a numSubsystems specifies the size of the \a subsystems array.
   *
   * This constructor does a number of things, such as allocating datastructures
   * to facilitate thread scheduling and communication, as well as allocating
   * a pool of threads to be used for the duration of the loop.
   *
   * Each subsystem is registered with the GameLoop, which is how the GameLoop
   * takes into account all of the Subsystem parameters and hints that need to
   * be known while the GameLoop is running.
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

    // loop through each subsystem
    for(size_t i = 0; i < m_numSubsystems; ++i) {
      if((m_subsystems[i]->hints() & Subsystem::Hints::HOG_THREAD) != Subsystem::Hints::NONE) {
        /*
         * Allocate a threadReady semaphore here. Normally ThreadPool would be
         * responsible for providing the semaphore, but now we need to provide
         * it. The Subsystem class takes ownership of both the readySemaphore
         * and the hogged thread, and it knows to de-allocate them in its
         * destructor.
         */
        SDL_sem *readySemaphore = SDL_CreateSemaphore(0);
        // create a hogged thread for each subsystem that needs one
        m_subsystems[i]->m_hoggedThread = new SubsystemThread(
            m_subsystems[i]->name(), m_state, readySemaphore);
      }
      // schedule each subsystem for initialization
      m_scheduleTask(Task(m_subsystems[i], Subsystem::Command::INIT), 0, Subsystem::Priority::HIGHEST);
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
      // for every task in the overdue queue
      while(m_overdueTaskQueue->hasNext()) {
        // try to run overdue tasks
      }

      // take a task from the task queue
      // check if the task can be run (i.e. threads are available)

      // if the task can't be scheduled now, add it to an overdue queue

      // FIXME: Need to actually wait for subsystems to be finished. This only hasn't crashed because drawing one triangle is fast. <_<
      for(size_t i = 0; i < m_numSubsystems; ++i) {
        m_threads->run(m_subsystems[i], Subsystem::Command::UPDATE);  // FIXME: don't actually schedule subsystems like this
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
      m_threads->run(m_subsystems[i], Subsystem::Command::CLOSE);
    }
  }

  /**
   * The m_scheduleTask routine schedules \a task (which is a Subsystem pointer
   * and a command) to be run in a thread at the given time \a gameTime with the
   * priority \a priority.
   * \var Task task the var
   */
  void GameLoop::m_scheduleTask(const Task &task, uint32_t gameTime, Subsystem::Priority priority) {
    assert(sizeof(Subsystem::Priority) == 4);

    uint64_t key = (static_cast<uint64_t>(gameTime) << 32) | static_cast<uint64_t>(priority);
    m_taskQueue->insert(key, task);
  }

  /**
   * The m_tryRunTask method attempts to run the given subsystem task. If the
   * task cannot be run at the moment (i.e. there are no suitable threads
   * available) then this method returns false. Otherwise, the task is started
   * on a thread and this method returns true.
   *
   * The m_tryRunTask method never blocks. This is vital as it allows the game
   * loop to continue to check if it might be possible to run other tasks with
   * the threads currently available.
   */
  bool GameLoop::m_tryRunTask(Task &task) {
    if((task.subsystem()->hints() & Subsystem::Hints::HOG_THREAD) != Subsystem::Hints::NONE) {
      // check if the subsystem's hogged thread is in use
      assert(task.subsystem()->m_hoggedThread != NULL);
      if(task.subsystem()->m_hoggedThread->isFree()) {
        // run the task on the subsystem's hogged thread
        return task.subsystem()->m_hoggedThread->tryRun(task.subsystem(), task.command());
      }
    }
    else {
      // TODO: try to run the task using the thread pool
    }
    return false;
  }
}
