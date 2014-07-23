#include "hoggedThread.h"

namespace tots {
  HoggedThread::HoggedThread(const char *name, const GameState *gameState) : SubsystemThread(gameState) {
    init(name);
  }

  HoggedThread::~HoggedThread() {
  }

  void HoggedThread::run(Subsystem *subsystem, SubsystemThread::Command command) {
    // assert that this thread is free and not running
    assert(this->isFree());

    // set the command
    m_command = command;  // TODO: move this into SubsystemThread

    // mark this thread as not free
    this->setFree(false);

    // signal the thread to start running
    signalRun();
  }

  int HoggedThread::m_run(void *voidSelf) {
    HoggedThread *self = static_cast<HoggedThread *>(voidSelf);
    while(!self->isDone()) {
      // mark this thread as free
      self->setFree(true);

      // wait for run signal (when the main thread wants us to run)
      self->waitRun();

      // TODO: automatically update the game state

      // run the subsystem
      self->runCurrentSubsystem(self->m_command);
    }
    return 0;
  }
}
