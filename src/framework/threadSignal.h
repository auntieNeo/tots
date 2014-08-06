#ifndef THREAD_SIGNAL_H_
#define THREAD_SIGNAL_H_

#include <SDL_mutex.h>

namespace tots { namespace framework {
  class ThreadSignal {
    public:
      ThreadSignal();
      ~ThreadSignal();

      void waitReady() { SDL_SemWait(m_readySemaphore); }
      bool tryWaitReady();
      bool checkReady() { return static_cast<bool>(SDL_SemValue(m_readySemaphore)); }

      void signalReady() { SDL_SemPost(m_readySemaphore); }

    private:
      SDL_sem *m_readySemaphore;
  };
} }

#endif
