#ifndef TOTS_FRAMEWORK_MESSAGE_H_
#define TOTS_FRAMEWORK_MESSAGE_H_

#include "../utility/crc32.h"
#include "../utility/variant.h"
#include "../utility/staticMap.h"

#define MESSAGE_MAX_ARGS 8

#define ADDRESS(x) static_cast<tots::utility::StringId>(COMPILE_TIME_CRC32_STR(x))
#define TYPE(x) static_cast<tots::utility::StringId>(COMPILE_TIME_CRC32_STR(x))

#include "../utility/stringId.h"

namespace tots { namespace framework {
  /**
   * There are three types of messages in Totsc:
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
  class Message {
    public:
      Message(const char *type);
      Message(utility::StringId type);
      ~Message();

      void setSender(utility::StringId sender) { m_sender = sender; }
      utility::StringId sender() const { return m_sender; }

      void setRecipiant(utility::StringId recipiant) { m_recipiant = recipiant; }
      utility::StringId recipiant() const { return m_recipiant; }

      bool operator==(const Message &other) const;
      const Message& operator=(const Message &other);

    private:
      utility::StaticMap<utility::StringId, utility::Variant, MESSAGE_MAX_ARGS> m_data;
      utility::StringId m_type, m_sender, m_recipiant;

      void m_copyFrom(const Message &other);

      uint32_t m_hashString(const char *string);
  };
} }

#endif
