#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include "../common.h"

namespace tots {
  class Event;
  class EventQueue {
    public:
      EventQueue();
      ~EventQueue();

      void appendEvent(Event *event);

    private:
      // list of events
      Event *m_events;
      size_t m_numEvents, m_maxEvents;

      // every queue reference counted
      size_t m_numReferences;
  };
}

#endif
