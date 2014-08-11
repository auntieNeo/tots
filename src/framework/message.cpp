#include "message.h"

#include "../utility/crc32.h"

using namespace tots::utility;

namespace tots { namespace framework {
  Message::Message(const char *type) {
    m_type = static_cast<StringId>(m_hashString(type));
  }

  Message::Message(StringId type) : m_type(type) {
  }

  Message::~Message() {
  }

  bool Message::operator==(const Message &other) const {
    return
      (this->m_type == other.m_type) &&
      (this->m_recipiant == other.m_recipiant) &&
      (this->m_sender == other.m_sender);
//      (this->m_data == other.m_data);  // FIXME: add this for variant
  }

  const Message& Message::operator=(const Message &other){
    if(this == &other) return *this;
    m_copyFrom(other);
    return *this;
  }

  void Message::m_copyFrom(const Message &other) {
    m_data = other.m_data;
    m_type = other.m_type;
    m_sender = other.m_sender;
    m_recipiant = other.m_recipiant;
  }

  uint32_t Message::m_hashString(const char *string) {
    return crc32_str(string);
  }
} }
