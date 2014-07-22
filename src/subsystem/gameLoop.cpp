#include "gameLoop.h"

#include "../graphics/graphics.h"
#include "../graphics/triangle.h"
#include "aggregateQueue.h"
#include "gameState.h"
#include "threadPool.h"

#define NUM_THREADS 4

namespace tots {
  GameLoop::GameLoop() {
  /* TODO:
   * Create a pool of threads.
   * Create a master GameState object.
   * Create GameState objects with those threads.
   * Copy the GameState object into each thread.
   */

    // create the master GameState object
    m_state = new GameState();  // FIXME: we might not even need this...

    // create the master message queue
    m_queue = new AggregateQueue();

    // TODO: populate the game state

    // create a pool of threads
    m_threads = new ThreadPool(NUM_THREADS);

    // create each of the subsystems
    // TODO: try to move subsystem-specific code out of here
    m_graphics = new Graphics();  // TODO: use factory method (for supporting Direct3D, etc.)
    m_threads->init(static_cast<Subsystem *>(m_graphics));

    // TODO: spawn this properly
    GraphicsTriangle *triangle = new GraphicsTriangle();
    m_graphics->addComponent(triangle);
  }

  GameLoop::~GameLoop() {
    delete m_graphics;
    delete m_threads;
    delete m_queue;
    delete m_state;
  }

  void GameLoop::run() {
    /* TODO:
     * Run the threads.
     * Combine CommandQueues into one master command queue.
     * Execute master command queue on each thread's GameState.
     * Run the threads again.
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
      m_threads->run(static_cast<Subsystem *>(m_graphics));

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

    // TODO: close all of the subsystems
    m_graphics->close();
  }
}
