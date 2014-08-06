#ifndef LOGGING_H_
#define LOGGING_H_

#include "../framework/subsystem.h"

namespace tots { namespace logging {
  class Logging : public framework::Subsystem {
    public:
      Logging();
      ~Logging();

    protected:
      virtual void m_init(const framework::GameState *state);
      virtual void m_update(const framework::GameState *state);
      virtual void m_close(const framework::GameState *state);
  };
} }

#endif
