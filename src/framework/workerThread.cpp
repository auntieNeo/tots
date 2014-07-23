#include "workerThread.h"

#include "gameState.h"
#include "subsystem.h"
#include "threadPool.h"

#include <SDL2/SDL_thread.h>

namespace tots {
  WorkerThread::WorkerThread(int threadIndex, ThreadPool *pool, const GameState *gameState) : SubsystemThread(gameState) {
    m_pool = pool;
    char *threadName = new char[256];
    snprintf(threadName, 256, "Worker Thread %02d", threadIndex);
    init(threadName);  // FIXME: this is terrible
    delete[] threadName;
  }

  WorkerThread::~WorkerThread() {
  }

  void WorkerThread::run(Subsystem *subsystem, SubsystemThread::Command command) {
    // assert that this thread is free and not running
    assert(this->isFree());

    // change the current subsystem
    setCurrentSubsystem(subsystem);
    m_command = command;  // TODO: move this into SubsystemThread

    // set the last thread for this subsystem
    subsystem->m_lastThread = this;

    // mark this thread as not free
    this->setFree(false);

    // signal the thread to start running
    signalRun();
  }

  int WorkerThread::m_run(void *voidSelf) {
    WorkerThread *self = static_cast<WorkerThread *>(voidSelf);
    while(!self->isDone()) {
      // mark this thread as free
      self->setFree(true);

      // let main thread know we're available
      self->m_pool->postThreadSemaphore();

      // wait for run signal (when the main thread wants us to run)
      self->waitRun();

      // TODO: automatically update the game state

      // run the subsystem
      self->runCurrentSubsystem(self->m_command);
    }
    return 0;
  }
}
