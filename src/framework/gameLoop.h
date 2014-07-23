#ifndef GAME_LOOP_H_
#define GAME_LOOP_H_

/**
 * GameLoop implements a non-locking game loop that shares data between
 * subsystem threads using an aggregate command queue and multiple copies of
 * the GameState object.
 */

namespace tots {
  class GameState;
  class AggregateQueue;

  class AudioSubsystem;
  class BehaviorSubsystem;
  class CollisionSubsystem;
  class Graphics;
  class PhysicsSubsystem;
  class ResourceSubsystem;

  class ThreadPool;

  class GameLoop {
    public:
      GameLoop();
      ~GameLoop();

      void run();

    private:
      GameState *m_state;
      AggregateQueue *m_queue;

      ResourceSubsystem *m_resource;
      BehaviorSubsystem *m_behavior;
      CollisionSubsystem *m_collision;
      PhysicsSubsystem *m_physics;
      Graphics *m_graphics;
      AudioSubsystem *m_audio;

      ThreadPool *m_threads;
  };
}

#endif
