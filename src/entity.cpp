#include <allegro5/allegro_primitives.h>

#include "behavior.h"
#include "entity.h"
#include "sprite.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace Tots
{
  Entity::Entity(const std::string &name, Type type, Sprite *sprite, Behavior *behavior)
  {
    m_name = name;
    m_x = 0;
    m_y = 0;
    m_xState = 0;
    m_yState = 0;
    m_sprite = sprite;
    m_behavior = behavior;
    m_currentState = Behavior::IDLE;
    m_frame = 0;
    m_ticks = 0;
    m_type = type;
  }

  Entity::~Entity()
  {
  }

  int Entity::width() const
  {
    return m_sprite->width();
  }

  int Entity::height() const
  {
    return m_sprite->height();
  }

  void Entity::setCurrentState(Behavior::StateFlag state)
  {
    m_currentState = state;
    m_xState = m_x;
    m_yState = m_y;
  }

  int Entity::numberOfStates() const
  {
    return m_sprite->numberOfStates();
  }

  /*!
   * \fn int Entity::tickCount()
   * Returns the total number of ticks passed since the state was changed.
   */

  Bitmask *Entity::bitmask()
  {
    return m_sprite->bitmask(m_currentState, m_frame);
  }

  bool Entity::collides(Entity *entity)
  {
//    return false;
    return bitmask()->collides(*entity->bitmask(), int(entity->x()) - int(m_x), int(entity->y()) - int(m_y));
  }

  void Entity::tick()
  {
    handleTick();
    m_behavior->tick(this);
    m_ticks++;
    m_collided = false;
    m_frame = 0;  // TODO: remove this and implement frames and states properly
  }

  void Entity::handleCollision(Entity *entity)
  {
    m_collided = true;
    m_frame = 1;  // TODO: remove this and implement frames and states properly
  }

  void Entity::draw(int x, int y) const
  {
    if(m_frame < m_sprite->numberOfFrames(m_currentState))
    {
      al_draw_bitmap(m_sprite->frame(m_currentState, m_frame), x, y, 1);
      /*
      ALLEGRO_BITMAP *redBox = al_load_bitmap("sprites/redBox-32x32.png");
      al_draw_bitmap(redBox, x, y, 0);
      al_destroy_bitmap(redBox);
      */
    }
    /*
    // draw a red X
    al_draw_line(x, y, x+m_width, y+m_height, al_map_rgb(0xFF, 0x00, 0x00), 3);
    al_draw_line(x, y+m_height, x+m_width, y, al_map_rgb(0xFF, 0x00, 0x00), 3);
    */
  }

  /*!
   * \brief Lua equivalent: Entity *Entity(String name, Tots::Entity::Type type, Sprite *sprite, Behavior *behavior)
   */
  int Entity_Entity(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 4)
      return 0;

    std::string name(lua_tostring(lua, 1));
    Tots::Entity::Type type = static_cast<Tots::Entity::Type>(lua_tointeger(lua, 2));
    Sprite *sprite = static_cast<Sprite*>(lua_touserdata(lua, 3));
    Behavior *behavior = static_cast<Behavior*>(lua_touserdata(lua, 4));
    lua_pushlightuserdata(lua, new Entity(name, type, sprite, behavior));

    return 1;
  }

  void Entity::registerLuaFunctions(lua_State *lua)
  {
    lua_register(lua, "Entity", Entity_Entity);
  }
}
