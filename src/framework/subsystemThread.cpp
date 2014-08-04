#include "subsystemThread.h"

#include "gameState.h"
#include "subsystem.h"
#include "threadPool.h"

#include <SDL_thread.h>

namespace tots {
  SubsystemThread::SubsystemThread(const char *name, const GameState *gameState, SDL_sem *readySemaphore) :
    m_currentTask(NULL), m_readySemaphore(readySemaphore) {
    // copy the game state from the given gameState
    m_gameState = new GameState(*gameState);

    // mark this thread as not free and not done (it's not even ready yet)
    setFree(false);
    setDone(false);

    // create an SDL semaphore to control our thread
    m_runSemaphore = SDL_CreateSemaphore(0);

    // create our SDL thread
    m_sdlThread = SDL_CreateThread(m_run, name, this);
  }

  void SubsystemThread::init(const char *name) {
  }

  SubsystemThread::~SubsystemThread() {
    // mark as done and wait for our thread
    setDone(true);
    SDL_WaitThread(m_sdlThread, NULL);

    // destroy our semaphore
    SDL_DestroySemaphore(m_runSemaphore);

    // delete our game state
    delete m_gameState;
  }

  bool SubsystemThread::tryRun(Task &task) {
    // check if the thread is free and not running
    if(this->isFree()) {
      // run the subsystem (non-blocking)
      run(task);
      return true;
    }
    return false;
  }

  void SubsystemThread::run(Task &task) {  // FIXME: figure out how task should be passed... probably by pointer
    // FIXME: maybe we should block here? or provide a blocking version?
    // assert that this thread is free and not running
    assert(this->isFree());

    // change the current subsystem
    this->setCurrentTask(&task);

    // set the last thread for this task
    task.setLastThread(this);

    // mark this thread as not free
    this->setFree(false);

    // signal the thread to start running
    this->signalRun();
  }

  int SubsystemThread::m_run(void *voidSelf) {
    SubsystemThread *self = static_cast<SubsystemThread *>(voidSelf);
    while(!self->isDone()) {
      // mark this thread as free
      self->setFree(true);

      // let main thread know we're available
      self->signalReady();

      // wait for run signal (when the main thread wants us to run)
      self->waitRun();

      // TODO: automatically update the game state
      // TODO: provide a callback to derived classes so that a SubsystemThread class can update the game state

      // run the current task
//      self->currentTask()->run();  // FIXME
      self->runCurrentSubsystem(self->currentTask()->command());  // FIXME
    }
    return 0;
  }
}
