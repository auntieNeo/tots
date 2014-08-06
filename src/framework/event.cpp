#include "event.h"

namespace tots { namespace framework {
  /**
   * There are three types of events in ToTS:
   *
   * The first type is an event directed at a particular subsystem.
   *
   * The second type is an event directed at each thread. These events typically
   * modify the GameState object.
   *
   * The third type of event is a broadcast event to multiple subsystems. A
   * reference count is placed on the event queue such that the queue is only
   * de-allocated once each intended subsystem has seen the events.
   */
} }
