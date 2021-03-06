#ifndef ENTITY_H_
#define ENTITY_H_

#include <string>

#include "behavior.h"
#include "bitmask.h"
#include "common.h"

namespace Tots
{ 
  class Sprite;

  class Entity
  {
    public:
      enum Type { NONE = 0, ENEMY, ENEMY_FIRE, PLAYER, PLAYER_FIRE, ITEM };

      enum StateCallback { STATE_BEGIN = 0, STATE_REPEAT, STATE_END, STATE_TICK, STATE_COLLISION };

      Entity(const std::string &name, Type type, Sprite *sprite, Behavior *behavior);
      virtual ~Entity();

      std::string name() const { return m_name; }
      double x() const { return m_x; }
      double xEndPoint() const { return m_x + width(); }
      void setX(double x) { m_x = x; }
      double y() const { return m_y; }
      double yEndPoint() const { return m_y + height(); }
      void setY(double y) { m_y = y; }
      int xAtStartOfState() const { return m_xState; }
      int yAtStartOfState() const { return m_yState; }
      // TODO: figure out how to make these inline without having header dependancy hell...
      int width() const;
      int height() const;
      Behavior::StateFlag currentState() const { return m_currentState; }
      void setCurrentState(Behavior::StateFlag state);
      Type type() { return m_type; };
      int numberOfStates() const;
      int tickCount() const { return m_ticks; }
      Sprite *sprite() { return m_sprite; }
      Bitmask *bitmask();
      bool collides(Entity *entity);
      size_t index() const { return m_index; }
      void setIndex(size_t index) { m_index = index; }

      void tick();
      virtual void handleTick() {}
      void handleCollision(Entity *entity);
      virtual void draw(int x, int y) const;

      static void registerLuaFunctions(lua_State *lua);

    protected:
      std::string m_name;

      Sprite *m_sprite;
      Behavior *m_behavior;

      double m_x, m_y;
      double m_xState, m_yState;  // TODO: rename these to m_xInitial and m_yInitial
      Behavior::StateFlag m_currentState;  // TODO: need to figure out how to store more data for the behavior, if necessary
      Type m_type;
      int m_frame, m_ticks;
      bool m_collided;
      size_t m_index;
  };
}

#endif
