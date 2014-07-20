#ifndef AGGREGATE_QUEUE_H_
#define AGGREGATE_QUEUE_H_

namespace tots {
  class EventQueue;
  class AggregateQueue {
    public:
      AggregateQueue();
      ~AggregateQueue();

      void append(EventQueue *queue);
  };
}

#endif
