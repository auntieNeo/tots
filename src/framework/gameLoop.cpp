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

      // TODO: possibly introduce determinism by implementing subsystem priority and a thread gate

    while(1) {
      // run each idle subsystem thread

      // FIXME: Need to actually wait for subsystems to be finished. This only hasn't crashed because drawing one triangle is fast. <_<
      for(size_t i = 0; i < m_numSubsystems; ++i) {
        m_threads->run(m_subsystems[i], SubsystemThread::UPDATE);  // FIXME: don't actually schedule subsystems like this
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
      m_threads->run(m_subsystems[i], SubsystemThread::CLOSE);
    }
  }
}
