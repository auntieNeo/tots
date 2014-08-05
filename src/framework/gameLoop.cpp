#include "gameLoop.h"

#include "gameState.h"
#include "taskQueue.h"
#include "threadPool.h"
#include "threadSignal.h"

#include <SDL_timer.h>

#include <unistd.h>

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
    // set some game loop parameter defaults
    setLoopHangCap(10000);
    setFramesPerSecond(60);

    // create an empty game state to share among the threads
    m_state = new GameState();  // FIXME: we might not even need to keep this around...

    // create the master message queue
//    m_messageQueue = new AggregateQueue();

    // create the task queue which schedules subsystems to run
    m_taskQueue = new TaskQueue(numSubsystems * 4);
    m_overdueTaskQueue = new TaskQueue(numSubsystems * 4);

    // TODO: populate the game state

    // create a signal to communicate with all of the threads
    m_signal = new ThreadSignal();

    // create a pool of threads
    m_threads = new ThreadPool(NUM_THREADS, m_state, m_signal);

    // copy the subsystem pointers
    m_numSubsystems = numSubsystems;
    m_subsystems = new Subsystem*[numSubsystems];
    memcpy(m_subsystems, subsystems, sizeof(Subsystem *) * numSubsystems);

    // loop through each subsystem
    for(size_t i = 0; i < m_numSubsystems; ++i) {
      if((m_subsystems[i]->hints() & Subsystem::Hints::HOG_THREAD) != Subsystem::Hints::NONE) {
        // create a hogged thread for each subsystem that needs one
        m_subsystems[i]->m_hoggedThread = new SubsystemThread(
            m_subsystems[i]->name(), m_state, m_signal);
      }
      // schedule each subsystem for initialization
      m_scheduleTask(Task(m_subsystems[i], Subsystem::Command::INIT), 0, Subsystem::Priority::HIGHEST);
    }
  }

  GameLoop::~GameLoop() {
    delete m_threads;
    delete m_signal;
    delete m_overdueTaskQueue;
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
    // FIXME: Need to actually wait for subsystems to be finished. This only hasn't crashed because drawing one triangle is fast. <_<

    // set up the time management variables
    uint32_t gameTime = 0;
    uint32_t frameCount = 0;
    uint32_t lastTime = SDL_GetTicks();
    double frameTimeAccumulator = 0.0f;

    uint32_t fpsTime = 0;
    uint32_t framesThisSecond = 0;

    while(1) {
      // update the time management variables
      uint32_t newTime = SDL_GetTicks();
      uint32_t elapsedTime = newTime - lastTime;
      lastTime = newTime;
      assert(elapsedTime <= 10000);
      if(elapsedTime > m_loopHangCap)
        // don't let the game loop hang into a "spiral of death"
        elapsedTime = m_loopHangCap;
      frameTimeAccumulator += elapsedTime;
      gameTime += elapsedTime;
      while(frameTimeAccumulator >= m_frameTime) {
        // add to the frame count
        // frameTimeAccumulator will keep the remainder
        // there is some rounding error, but it should be fine
        frameTimeAccumulator -= m_frameTime;
        frameCount += 1;
      }
      // calculate frames per second
      fpsTime += elapsedTime;
      if(fpsTime >= 1000) {
        printf("frames per second: %d\n", framesThisSecond);
        fpsTime = framesThisSecond = 0;
      }

      /*
      // FIXME: this design falls apart if the highest priority overdue task can't be run, but the next highest priority task can be run
      if(m_overdueTaskQueue->hasNext()) {
        printf("have an overdue task\n");
        // try to run the highest priority overdue task
        // FIXME: mad code duplication in here
        // check if the task is due
        uint32_t nextTaskTime = m_overdueTaskQueue->peekNextKey().m_data.m_time;
        Subsystem::Priority nextTaskPriority = m_overdueTaskQueue->peekNextKey().m_data.m_priority;
        if(nextTaskTime <= gameTime) {
          // take the task from the task queue
          Task nextTask = m_overdueTaskQueue->popNext();
          // try to run the task
          if(m_tryRunTask(nextTask)) {
            // TODO: check for update period hints
            // schedule the next task based on the subsystem's update period
            m_scheduleTask(Task(nextTask.subsystem(), Subsystem::Command::UPDATE),
                nextTaskTime + nextTask.subsystem()->updatePeriod(),
                Subsystem::Priority::NORMAL,
                m_overdueTaskQueue);
            assert(m_overdueTaskQueue->hasNext());
          }
          else {
            // add the task to the overdue task queue if we could not run it
            // (i.e. no suitable threads were available)
            m_scheduleTask(nextTask, nextTaskTime, nextTaskPriority, m_overdueTaskQueue);
            printf("well, crap.\n");
          }
        }
      }
      */

      /*
       * There are three main cases that the game loop has to account for when
       * scheduling tasks on threads.
       *
       * The first case is the easiest. If there are no thread resources
       * available the game loop must wait for them to become available.
       *
       * The second case is also fairly easy. If there are thread resources
       * available, and there is a task in the task queue that can be run with
       * those resources, then the game loop signals a thread to run that task.
       *
       * These first two cases are both solved with the call to
       * m_signal->waitReady().
       *
       * The third case is the most complicated. If there are thread resources
       * available, but there are no tasks in the task queue that can be run
       * with those resources, then game loop must either wait for a running
       * thread to complete, or it must sleep to give other threads a chance to
       * finish their execution. A busy wait does not work here.
       */

      // wait patiently for thread resources to become available
      m_signal->waitReady();

      // now we need to determine if there is a task that we can run
      // check the first task in the task queue
      if(m_taskQueue->hasNext()) {
        // check if the task is due
        uint32_t nextTaskTime = m_taskQueue->peekNextKey().m_data.m_time;
        Subsystem::Priority nextTaskPriority = m_taskQueue->peekNextKey().m_data.m_priority;
        if(nextTaskTime <= gameTime) {
          // take the task from the task queue
          Task nextTask = m_taskQueue->popNext();
          // try to run the task
          if(m_tryRunTask(nextTask)) {
            framesThisSecond += 1;
            // TODO: check for update period hints
            // schedule the next task based on the subsystem's update period
            m_scheduleTask(Task(nextTask.subsystem(), Subsystem::Command::UPDATE),
                nextTaskTime + nextTask.subsystem()->updatePeriod(),
                Subsystem::Priority::NORMAL,
                m_taskQueue);
            assert(m_taskQueue->hasNext());
          }
          else {
            // add the task to the overdue task queue if we could not run it
            // (i.e. no suitable threads were available)
            m_scheduleTask(nextTask, nextTaskTime, nextTaskPriority, m_taskQueue /*FIXME*/);
            assert(m_taskQueue->hasNext());
          }
        }
      }

      // sleep to let other threads run
//      SDL_Delay(1);  // FIXME: do this some other way

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

    // TODO: close all of the subsystems
    /*
    for(size_t i = 0; i < m_numSubsystems; ++i) {
      m_threads->run(m_subsystems[i], Subsystem::Command::CLOSE);
    }
    */
  }

  void GameLoop::m_scheduleTask(const Task &task, uint32_t gameTime, Subsystem::Priority priority, TaskQueue *queue) {
    assert(sizeof(Subsystem::Priority) == 4);

    TaskQueueKey key;
    key.m_data.m_time = gameTime;
    key.m_data.m_priority = priority;
    queue->insert(key, task);
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
        return task.subsystem()->m_hoggedThread->tryRun(task);
      }
    }
    // try to run the task through the thread pool
    return m_threads->tryRun(task);
  }
}
