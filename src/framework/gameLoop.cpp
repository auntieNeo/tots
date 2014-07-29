#include "gameLoop.h"

#include "aggregateQueue.h"
#include "gameState.h"
#include "threadPool.h"
#include "subsystem.h"

#define NUM_THREADS 1

namespace tots {
  GameLoop::GameLoop(Subsystem **subsystems, size_t numSubsystems) {
    // create an empty game state to share among the threads
    m_state = new GameState();  // FIXME: we might not even need to keep this around...

    // create the master message queue
    m_queue = new AggregateQueue();

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
      m_threads->run(m_subsystems[i], SubsystemThread::INIT);
    }
  }

  GameLoop::~GameLoop() {
    delete m_threads;
    delete m_queue;
    delete m_state;
    delete[] m_subsystems;
  }

  void GameLoop::run() {
    /* TODO:
     * Combine CommandQueues into one master command queue.
     * Execute master command queue on each thread's GameState.
     */

    while(1) {
      // run each idle subsystem thread

      // resource run schedule:
      // TODO: run the resource thread whenever resource events occur
      // most threads besides graphics will idle until the resource thread is done
      // m_threads.run(m_resource);

      // game simulation run schedule:
      // fixed to simulation tick once per frame
      // TODO: run N collision threads and the behavior thread (blocking)
//      m_threads.run(m_collision);
//      m_threads.run(m_behavior);
      // TODO: join behavior thread and run the physics thread
//      if(m_threads.join(m_behavior))
//        m_threads.run(m_physics);

      // graphics run schedule:
      // run graphics thread for every frame
      // FIXME: Need to actually wait for subsystems to be finished. This only hasn't crashed because drawing one triangle is fast. <_<
      for(size_t i = 0; i < m_numSubsystems; ++i) {
        m_threads->run(m_subsystems[i], SubsystemThread::UPDATE);  // FIXME: don't actually schedule subsystems like this
      }

      // audio run schedule:
      // TODO: run the audio thread whenever possible at least once per frame
      //m_threads.run(m_audio);

      // TODO: wait for any subsystem thread
      // TODO: possibly introduce determinism by implementing subsystem priority and a thread gate
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
      m_threads->run(m_subsystems[i], SubsystemThread::CLOSE);
    }
  }
}
