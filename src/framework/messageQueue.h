#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

namespace tots { namespace framework {
  class MessageQueue : public AggregateQueue<Message> {
    public:
      MessageQueue();
      ~MessageQueue();

      void sendMessage();
  };
} }

#endif
