#include "threadSignal.h"

#include "../log.h"
#include "../common.h"

namespace tots { namespace framework {
  ThreadSignal::ThreadSignal() {
    m_readySemaphore = SDL_CreateSemaphore(0);
  }

  ThreadSignal::~ThreadSignal() {
  }

  bool ThreadSignal::tryWaitReady() {
    int response = SDL_SemTryWait(m_readySemaphore);
    if(response == 0)
      return true;
    else if(response == SDL_MUTEX_TIMEDOUT)
      return false;
    if(response < 0) {
      log_SDL_error("Error waiting for semaphore");
      exit(1);  // FIXME: abort properly
    }
    assert(false);  // if we get here, there's an SDL flag I don't know about
    return false;
  }
} }
