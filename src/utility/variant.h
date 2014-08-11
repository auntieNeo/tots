#ifndef TOTS_UTILITY_VARIANT_H_
#define TOTS_UTILITY_VARIANT_H_

#include "stringId.h"

namespace tots { namespace utility {
  class Variant {
    public:
      enum class Type : uint32_t {
        BOOL,
        FLOAT,
        INTEGER,
        STRING_ID
      };

      union Value {
      } m_value;

      void setValue(bool value) { m_bool = value; }
      void setValue(float value) { m_float = value; }
      void setValue(uint32_t value) { m_integer = value; }
      void setValue(StringId value) { m_stringId = value; }

      bool toBool() const { return m_bool; }
      uint32_t toInteger() const { return m_integer; }
      float toFloat() const { return m_float; }
      StringId toStringId() const { return m_stringId; }

    private:
      union {
        bool m_bool;
        float m_float;
        uint32_t m_integer;
        StringId m_stringId;
      };
      Type m_type;
  };
} }

#endif
