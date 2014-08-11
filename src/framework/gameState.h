#ifndef GAME_STATE_H_
#define GAME_STATE_H_

#include "../common.h"

namespace tots { namespace framework {
  class MessageQueue;
  class GameState {
    public:
      GameState();
      ~GameState();

      MessageQueue *messageQueue() const { return m_messages; }

    private:
      // the message queue
      MessageQueue *m_messages;

      // internal state
//      std::vector<GameObject *> m_gameObjects;
      uint32_t m_currentState;

      // internal update methods
      void m_updateMessageQueue(const MessageQueue *messages);
      void m_updateState();
  };
} }

#endif
