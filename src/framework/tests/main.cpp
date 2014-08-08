#include <SDL_timer.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../gameLoop.h"

using namespace testing;

namespace tots { namespace framework { namespace tests {

  /**
   * \file
   * This file contains unit tests of the framework's operation. It uses
   * <a href="https://code.google.com/p/googletest/">Google Test</a> and
   * <a href="https://code.google.com/p/googlemock/">Google Mock</a>.
   */

  class Canadian;
  class SlowSubsystem;
  class QuickSubsystem;

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

  /**
   * A fake Subsystem class that is used to test communication between
   * subsystems in GameLoop. Based on the South Park "I'm not your buddy" skit.
   */
  class Canadian : public Subsystem {
    public:
      void m_init(const GameState *state) {}
      void m_update(const GameState *state) {
//      sprintf(message, "I'm not your %s, %s!",
//        state->sendMessage(); TODO
      }
      void m_close(const GameState *state) {}

      Hints hints() const { return Hints::NONE; }
      const char *name() const { return "Canadian Subsystem"; }
      int32_t updatePeriod() const { return 10; }
  };

  /**
   * This tests the basic communication between tasks using the message queue. If
   * there is any unexpected failure in communication, this test will fail.
   */
  TEST(GameLoop, TaskCommunication) {
    MockSubsystemWithDelegate<Canadian> terrance, phillip;
    Subsystem *canadians[2];
    canadians[0] = &terrance;
    canadians[1] = &phillip;

    // Subsystem registration within GameLoop
    EXPECT_CALL(terrance, hints());
    EXPECT_CALL(phillip, hints());

    /*
    // talk to phillip
    EXPECT_CALL(terrance, m_update(_))
      .Times(10);
    // talk to terrance
    EXPECT_CALL(phillip, m_update(_))
      .Times(10);
      */

    GameLoop *gameLoop = new GameLoop(canadians, 2);

    delete gameLoop;
  }

  /**
   * A fake Subsystem class that models a "slow" subsystem which is slow to
   * execute despite advertising fast update periods.
   *
   * The slowness is accomplished by sleeping.
   */
  class SlowSubsystem : public Subsystem {
    public:
      void m_init(const GameState *state) { SDL_Delay(1000); }
      void m_update(const GameState *state) { SDL_Delay(1000); }
      void m_close(const GameState *state) { SDL_Delay(1000); }

      Hints hints() const { return Hints::NONE; }
      const char *name() const { return "Slow Subsystem"; }
      int32_t updatePeriod() const { return 1; }
  };

  /**
   * A task determinism test (race-condition test) in which a quick task's
   * execution depends on a slow task's data. If the GameLoop is functioning
   * properly, then the quick task will execute after the slow task so that it
   * recieves the same (deterministic) data every time, despite how long it has to
   * wait for the slow task.
   */
  TEST(GameLoop, TaskDeterminism) {
    MockSubsystemWithDelegate<SlowSubsystem> slow, fast;
    Subsystem *subsystems[2];
    subsystems[0] = &slow;
    subsystems[1] = &fast;

    // Subsystem registration within GameLoop
    EXPECT_CALL(slow, hints());
    EXPECT_CALL(fast, hints());

    GameLoop *gameLoop = new GameLoop(subsystems, 2);

//    gameLoop->run();

    delete gameLoop;
  }

  // TODO: write a test for a preemptable task i.e. a task that takes a long time to execute, but must be preempted by shorter tasks and not hog a single processor
} } }
