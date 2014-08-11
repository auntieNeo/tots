#include "gameState.h"
#include "messageQueue.h"

namespace tots { namespace framework {
  GameState::GameState() {
  }

  GameState::~GameState() {
  }

  /**
   * Updates GameQueue's internal message queue from the given message queue.
   * Messages with timestamps (in game time) up to the given gameTime parameter
   * will be copied. This method is to be run in the main thread.
   */
  void m_updateMessageQueue(const MessageQueue *messages, uint32_t gameTime) {
  }

  /**
   * Updates the internal state of GameState based on messages addressed to
   * GameState in the message queue. This method is to be run in a task thread.
   */
  void m_updateState() {
  }
} }
