#include <string>

#include "behavior.h"
#include "level.h"
#include "luaFunctions.h"
#include "world.h"

#include "entity.h"
#include "path.h"

#include <iostream>
using namespace std;

namespace Tots
{
  // void addDistanceEvent(Level *level, int pixels, const char *luaFunction)
  int Level_addDistanceEvent(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 3)
      return 0;

    int pixels = lua_tointeger(lua, 2);
    const char *luaFunction = lua_tostring(lua, 3);
    static_cast<Level*>(lua_touserdata(lua, 1))->addDistanceEvent(pixels, std::string(luaFunction));

    return 0;
  }

  // void addTimedEvent(Level *level, double seconds, const char *luaFunction)
  int Level_addTimedEvent(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 3)
      return 0;

    double seconds = lua_tonumber(lua, 2);
    const char *luaFunction = lua_tostring(lua, 3);
    static_cast<Level*>(lua_touserdata(lua, 1))->addTimedEvent(seconds, std::string(luaFunction));

    return 0;
  }

  // FIXME: there doesn't need to be a world parameter... just get the world lua object using whatever lua calls you need
  // void spawnEntity(World *world, int x, int y, Entity *entity)
  int World_spawnEntity(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 4)
      return 0;

    int x = lua_tointeger(lua, 2);
    int y = lua_tointeger(lua, 3);
    Entity *entity = static_cast<Entity*>(lua_touserdata(lua, 4));
    static_cast<World*>(lua_touserdata(lua, 1))->spawnEntity(x, y, entity);

    return 0;
  }


  // void addSprite(Level *level, Sprite *sprite);
  int Level_addSprite(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    static_cast<Level*>(lua_touserdata(lua, 1))->addSprite(static_cast<Sprite*>(lua_touserdata(lua, 2)));

    return 0;
  }

  // *** the new and improved lua functions ***
  /*!
   * \addtogroup lua Lua Bindings
   * \todo Add detailed Lua binding documentation/tutorial here.
   * @{
   */

  /*!
   * \brief Lua equivalent: void addSprite(Sprite *sprite)
   * \sa getSprite()
   */
  int addSprite(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    Sprite *sprite = static_cast<Sprite*>(lua_touserdata(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    level->addSprite(sprite);

    return 0;
  }

  /*!
   * \brief Lua equivalent: Sprite *getSprite(String name)
   * \sa addSprite()
   */
  int getSprite(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    std::string name(lua_tostring(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    lua_pushlightuserdata(lua, level->getSprite(name));

    return 1;
  }

  /*!
   * \brief Lua equivalent: void addBehaviorState(EntityState *state)
   * \sa getBehaviorState()
   */
  int addBehaviorState(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
    {
      cout << "addBehaviorState lua oops" << endl;
      return 0;
    }

    Behavior::State *state = static_cast<Behavior::State*>(lua_touserdata(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    level->addBehaviorState(state);

    return 0;
  }

  /*!
   * \brief Lua equivalent: Behavior::State *getBehaviorState(String name)
   * \sa addBehaviorState()
   */
  int getBehaviorState(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    std::string name(lua_tostring(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    lua_pushlightuserdata(lua, level->getBehaviorState(name));

    return 1;
  }

  /*!
   * \brief Lua equivalent: void addBehavior(Behavior *behavior)
   * \sa getBehavior(), Behavior()
   */
  int addBehavior(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    Behavior *behavior = static_cast<Behavior*>(lua_touserdata(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    level->addBehavior(behavior);

    return 0;
  }

  /*!
   * \brief Lua equivalent: Behavior *getBehavior(String name)
   * \sa addBehavior()
   */
  int getBehavior(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    std::string name(lua_tostring(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    lua_pushlightuserdata(lua, level->getBehavior(name));

    return 1;
  }

  /*!
   * \brief Lua equivalent: void addEntity(Entity *entity)
   * \sa copyEntity(), Entity()
   */
  int addEntity(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    Entity *entity = static_cast<Entity*>(lua_touserdata(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    level->addEntity(entity);

    return 0;
  }

  /*!
   * \brief Lua equivalent: Entity *copyEntity(String name)
   * \sa addEntity()
   */
  int copyEntity(lua_State *lua)
  {
    lua_getglobal(lua, "level");

    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    std::string name(lua_tostring(lua, 1));
    Level *level = static_cast<Level*>(lua_touserdata(lua, 2));
    lua_pushlightuserdata(lua, level->copyEntity(name));

    return 1;
  }

  /*!
   * \brief Lua equivalent: void spawnEntity(int x, int y, Entity *entity)
   */
  int spawnEntity(lua_State *lua)
  {
    lua_getglobal(lua, "world");

    int n = lua_gettop(lua);

    if(n != 4)
      return 0;

    int x = lua_tointeger(lua, 1);
    int y = lua_tointeger(lua, 2);
    Entity *entity = static_cast<Entity*>(lua_touserdata(lua, 3));
    World *world = static_cast<World*>(lua_touserdata(lua, 4));
    world->spawnEntity(x, y, entity);

    return 0;
  }

  /*!
   * \brief Lua equivalent: Sprite *Sprite(const std::string &name, const std::string &file, int width, int height, int numberOfStates, const char *numberOfFrames, const char *framesPerSecond, int framesPerRow);
   */
  int Sprite_Sprite(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n < 5)
      return 0;
    int numberOfStates = lua_tointeger(lua, 5);
    if(n != 6 + 2 * numberOfStates)
      return 0;

    std::string name(lua_tostring(lua, 1));
    std::string file(lua_tostring(lua, 2));
    int width = lua_tointeger(lua, 3);
    int height = lua_tointeger(lua, 4);
    char *numberOfFrames = new char[numberOfStates];
    char *framesPerSecond = new char[numberOfStates];
    for(int i = 0; i < numberOfStates; i++)
    {
      numberOfFrames[i] = lua_tointeger(lua, i + 6);
      framesPerSecond[i] = lua_tointeger(lua, i + 6 + numberOfStates);
    }
    int framesPerRow = lua_tointeger(lua, n);

    lua_pushlightuserdata(lua, new Sprite(name, file, width, height, numberOfStates, numberOfFrames, framesPerSecond, framesPerRow));

    delete numberOfFrames;
    delete framesPerSecond;

    return 1;
  }

  /*!
   * \brief Lua equivalent: Behavior *Behavior(String name, int numberOfStates)
   */
  int Behavior_Behavior(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 2)
      return 0;

    std::string name(lua_tostring(lua, 1));
    int numberOfStates = lua_tointeger(lua, 2);
    lua_pushlightuserdata(lua, new Behavior(name, numberOfStates));

    return 1;
  }

  /*!
   * \brief Lua equivalent: void Behavior_addState(Behavior *behavior, Tots::Entity::StateFlag flag, EntityState *state)
  */
  int Behavior_addState(lua_State *lua)
  {
    int n = lua_gettop(lua);

    if(n != 3)
      return 0;

    Behavior *behavior = static_cast<Behavior*>(lua_touserdata(lua, 1));
    Tots::Behavior::StateFlag flag = static_cast<Tots::Behavior::StateFlag>(lua_tointeger(lua, 2));
    Tots::Behavior::State *state = static_cast<Behavior::State*>(lua_touserdata(lua, 3));
    behavior->addState(flag, state);

    return 0;
  }

  void registerLuaFunctions(lua_State *lua)
  {
    // TODO: put some important constants into lua, especially the Entity::State enum
    lua_register(lua, "addDistanceEvent", Level_addDistanceEvent);
    lua_register(lua, "addTimedEvent", Level_addTimedEvent);

    // *** new and improved ***
    lua_register(lua, "addSprite", addSprite);
    lua_register(lua, "getSprite", getSprite);
    lua_register(lua, "addBehaviorState", addBehaviorState);
    lua_register(lua, "getBehaviorState", getBehaviorState);
    lua_register(lua, "addBehavior", addBehavior);
    lua_register(lua, "getBehavior", getBehavior);
    lua_register(lua, "addEntity", addEntity);
    lua_register(lua, "copyEntity", copyEntity);
    lua_register(lua, "spawnEntity", spawnEntity);

    lua_register(lua, "Sprite", Sprite_Sprite);

    lua_register(lua, "Behavior", Behavior_Behavior);
    lua_register(lua, "Behavior_addState", Behavior_addState);

    Entity::registerLuaFunctions(lua);
    Path::registerLuaFunctions(lua);
  }
  /*! @} */
}
