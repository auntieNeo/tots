#include "logging.h"

using namespace tots::framework;

namespace tots { namespace logging {
  Logging::Logging() {
  }

  Logging::~Logging() {
  }

  void Logging::m_init(const GameState *state) {
  }

  void Logging::m_update(const GameState *state) {
    // TODO: read log events from the message queue
  }

  void Logging::m_close(const GameState *state) {
  }
} }
