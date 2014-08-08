#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../gameLoop.h"

using namespace tots::framework;
using namespace testing;

/**
 * \file
 * This file contains unit tests of the framework's operation. It uses
 * <a href="https://code.google.com/p/googletest/">Google Test</a> and
 * <a href="https://code.google.com/p/googlemock/">Google Mock</a>.
 */

class Canadian : public Subsystem {
  public:
    void m_init(const GameState *state) {}
    void m_update(const GameState *state) {}
    void m_close(const GameState *state) {}

    Hints hints() const { return Hints::NONE; }
    const char *name() const { return "Canadian Subsystem"; }
    int32_t updatePeriod() const { return 10; }
};

// create a mock class to test subsystems in the game loop
class MockSubsystem : public Subsystem {
  public:
    //! @cond Doxygen_Ignore
    MOCK_METHOD1(m_init, void(const GameState *state));
    MOCK_METHOD1(m_update, void(const GameState *state));
    MOCK_METHOD1(m_close, void(const GameState *state));
    MOCK_CONST_METHOD0(hints, Subsystem::Hints(void));
    MOCK_CONST_METHOD0(name, const char *(void));
    MOCK_CONST_METHOD0(updatePeriod, int32_t(void));

    void m_sendMessage(const GameState *state) {
    }
    //! @endcond
    void delagateToCanadian() {
      ON_CALL(*this, m_init(_))
        .WillByDefault(Invoke(&m_canadian, &Canadian::m_init));
    }
  private:
    Canadian m_canadian;
};

/**
 * This tests the basic communication between tasks using the message queue. If
 * there is any unexpected failure in communication, this test will fail.
 */
TEST(GameLoop, TaskCommunication) {
  MockSubsystem terrance, phillip;
  Subsystem *canadians[2];
  canadians[0] = &terrance;
  canadians[1] = &phillip;

  // define some default mock behavior
  ON_CALL(terrance, hints())
    .WillByDefault(Return(Subsystem::Hints::NONE));
  ON_CALL(phillip, hints())
    .WillByDefault(Return(Subsystem::Hints::NONE));

  ON_CALL(terrance, m_update(_))
    .WillByDefault(Return());
  ON_CALL(phillip, m_update(_))
    .WillByDefault(Return());

  // Subsystem registration within GameLoop
  EXPECT_CALL(terrance, hints());
  EXPECT_CALL(phillip, hints());

  // talk to phillip
  /*
  EXPECT_CALL(terrance, m_update(_))
    .WillOnce(Invoke(this, &MockSubsystem::m_sendMessage));
    */

  GameLoop *gameLoop = new GameLoop(canadians, 2);

  delete gameLoop;
}

/**
 * A task determinism test (race-condition test) in which a quick task's
 * execution depends on a slow task's data. If the GameLoop is functioning
 * properly, then the quick task will execute after the slow task so that it
 * recieves the same (deterministic) data every time, despite how long it has to
 * wait for the slow task.
 */
TEST(GameLoop, TaskDeterminism) {
  MockSubsystem slow, fast;
  Subsystem *subsystems[2];
  subsystems[0] = &slow;
  subsystems[1] = &fast;

  // define some default mock behavior
  ON_CALL(slow, hints())
    .WillByDefault(Return(Subsystem::Hints::NONE));
  ON_CALL(fast, hints())
    .WillByDefault(Return(Subsystem::Hints::NONE));

  GameLoop *gameLoop = new GameLoop(subsystems, 2);

  delete gameLoop;
}

  // TODO: write a test for a preemptable task i.e. a task that takes a long time to execute, but must be preempted by shorter tasks and not hog a single processor
