#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../subsystem.h"

/**
 * \file
 * This file contains unit tests of the framework's operation. It uses
 * <a href="https://code.google.com/p/googletest/">Google Test</a> and
 * <a href="https://code.google.com/p/googlemock/">Google Mock</a>.
 */

namespace tots { namespace framework { namespace tests {
  using namespace testing;

  /**
   * A mock class used to test the GameLoop class' interaction with Subsystem
   * objects.
   */
  class MockSubsystem : public Subsystem {
    public:
      //! @cond Doxygen_Ignore
      MOCK_METHOD1(m_init, void(const GameState *state));
      MOCK_METHOD1(m_update, void(const GameState *state));
      MOCK_METHOD1(m_close, void(const GameState *state));
      MOCK_CONST_METHOD0(hints, Subsystem::Hints(void));
      MOCK_CONST_METHOD0(name, const char *(void));
      MOCK_CONST_METHOD0(updatePeriod, int32_t(void));
      //! @endcond
  };

  /**
   * A mock Subsystem class that delegates behavior to the class of the given
   * template parameter.
   */
  template<typename T>
  class MockSubsystemWithDelegate : public MockSubsystem {
    public:
      MockSubsystemWithDelegate() {
        m_delegateBehavior();
      }

    private:
      T m_delegate;

      void m_delegateBehavior() {
        ON_CALL(*this, m_init(_))
          .WillByDefault(Invoke(&m_delegate, &T::m_init));
        ON_CALL(*this, m_update(_))
          .WillByDefault(Invoke(&m_delegate, &T::m_update));
        ON_CALL(*this, m_close(_))
          .WillByDefault(Invoke(&m_delegate, &T::m_close));
        ON_CALL(*this, hints())
          .WillByDefault(Invoke(&m_delegate, &T::hints));
        ON_CALL(*this, name())
          .WillByDefault(Invoke(&m_delegate, &T::name));
        ON_CALL(*this, updatePeriod())
          .WillByDefault(Invoke(&m_delegate, &T::updatePeriod));
      }
  };
} } }

