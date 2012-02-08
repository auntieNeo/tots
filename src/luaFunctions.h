#ifndef LUA_FUNCTIONS_H_
#define LUA_FUNCTIONS_H_

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace Tots
{
  // void addDistanceEvent(Level *level, int pixels, const char *luaFunction)
  int Level_addDistanceEvent(lua_State *lua);

  // void addTimedEvent(Level *level, double seconds, const char *luaFunction)
  int Level_addTimedEvent(lua_State *lua);

  // void spawnEntity(World *world, int x, int y, Entity *entity)
  int World_spawnEntity(lua_State *lua);


  // void addSprite(Level *level, Sprite *sprite);
  int Level_addSprite(lua_State *lua);

    /*
    // Enemy *Enemy::Enemy(blah)
    int Enemey_Enemy(blah);

    // Path *Path::Path(blah)
    int Enemey_Path(blah);
    */

  // *** the new and improved lua functions ***
  int addSprite(lua_State *lua);
  int getSprite(lua_State *lua);
  int addMovement(lua_State *lua);
  int getMovement(lua_State *lua);
  int addBehavior(lua_State *lua);
  int getBehavior(lua_State *lua);
  int addEntity(lua_State *lua);
  int copyEntity(lua_State *lua);
  int spawnEntity(lua_State *lua);

  int Sprite_Sprite(lua_State *lua);

  int Path_Path(lua_State *lua);
  int Path_lineTo(lua_State *lua);
  int Path_quadTo(lua_State *lua);
  int Path_cubicTo(lua_State *lua);

  int Behavior_Behavior(lua_State *lua);
  int Behavior_setMovement(lua_State *lua);

  int Entity_Entity(lua_State *lua);

  void registerLuaFunctions(lua_State *lua);
}

#endif
