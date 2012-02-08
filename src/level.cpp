#include <string>

#include "behavior.h"
#include "level.h"
#include "luaFunctions.h"

#include <iostream>
using namespace std;

/*! \file */

namespace Tots
{
  /*! \class Level
   * Level is responsible for telling World what to do. It keeps track of the events added by lua scripts and calls them. It also stores the sprites, movements, behaviors, and (un-spawned) entities.
   */
  Level::Level(const std::string &file)
  {
    m_world = new World(this);  // FIXME: move this back to the top of the constructor. srsly, lua might need it

    m_lua = lua_open();
    luaL_openlibs(m_lua);
    registerLuaFunctions(m_lua);
    lua_pushlightuserdata(m_lua, this);
    lua_setglobal(m_lua, "level");
    lua_pushlightuserdata(m_lua, m_world);
    lua_setglobal(m_lua, "world");
    luaL_dofile(m_lua, file.c_str());
    lua_getglobal(m_lua, "initLevel");
    lua_call(m_lua, 0, 0);
  }

  Level::~Level()
  {
    // TODO: delete the sprites and entities and stuff
    lua_close(m_lua);
    delete m_world;
  }

  void Level::tick()
  {
    m_world->tick();
  }

  void Level::draw()
  {
    m_world->draw();
  }

  void Level::addDistanceEvent(int pixels, const std::string &luaFunction)
  {
    DistanceEvent event;
    event.pixels = pixels;
    event.luaFunction = luaFunction;
    m_distanceEvents.push_back(event);
    m_distanceEvents.sort(compareDistance);
  }

  void Level::addTimedEvent(double seconds, const std::string &luaFunction)
  {
    TimedEvent event;
    event.seconds = seconds;
    event.luaFunction = luaFunction;
    m_timedEvents.push_back(event);
    m_timedEvents.sort(compareTimed);
  }

  void Level::changeDistance(int newDistance, World *world)
  {
    if(m_distanceEvents.empty())
      return;

    if(newDistance >= m_distanceEvents.front().pixels)
    {
      lua_getglobal(m_lua, m_distanceEvents.front().luaFunction.c_str());
      lua_pushlightuserdata(m_lua, world);
      lua_call(m_lua, 1, 0);

      m_distanceEvents.pop_front();

      // check again in case multiple events have been triggered
      this->changeDistance(newDistance, world);
    }
  }

  void Level::changeTime(double newTime, World *world)
  {
    if(m_timedEvents.empty())
      return;

    if(newTime >= m_timedEvents.front().seconds)
    {
      lua_getglobal(m_lua, m_timedEvents.front().luaFunction.c_str());
      lua_pushlightuserdata(m_lua, world);
      lua_call(m_lua, 1, 0);

      m_timedEvents.pop_front();

      // check again in case multiple events have been triggered
      this->changeTime(newTime, world);
    }
  }

  /*!
   * Adds a sprite to the sprites made available to entities by Level.
   *
   * Level assumes ownership of \a sprite. \a sprite is deleted if a sprite of the same name has already been added.
   *
   * \sa getSprite()
   */
  void Level::addSprite(Sprite *sprite)
  {
    if(m_sprites.find(sprite->name()) != m_sprites.end())
    {
      // a sprite with the same name already exists
      delete sprite;
      return;
    }
    m_sprites[sprite->name()] = sprite;
  }

  /*!
   * Returns the sprite stored in the Level that has the string \a name as its name.
   *
   * Returns NULL if no such sprite was found.
   *
   * \sa addSprite()
   */
  Sprite *Level::getSprite(const std::string &name)
  {
    if(m_sprites.find(name) == m_sprites.end())  // FIXME: should use the iterator to return the entity without having to find it twice
    {
      return NULL;
    }
    return m_sprites[name];
  }

  /*!
   * Adds a movement to the movements made available to entities by Level.
   *
   * Level assumes ownership of \a movement. \a movement is deleted if a movement of the same name has already been added.
   *
   * \sa getMovement()
   */
  void Level::addMovement(Movement *movement)
  {
    if(m_movements.find(movement->name()) != m_movements.end())
    {
      cout << "addMovement oops" << endl;
      // a movement with the same name already exists
      delete movement;
      return;
    }
    cout << "addMovement movement: " << (void *)movement << endl;
    cout << "addMovement name: " << movement->name() << endl;
    m_movements[movement->name()] = movement;
  }

  /*!
   * Returns the movement stored in the Level that has the string \a name as its name.
   *
   * Returns NULL if no such movement was found.
   *
   * \sa addMovement()
   */
  Movement *Level::getMovement(const std::string &name)
  {
    cout << "getMovement" << endl;
    std::map<std::string,Movement*>::iterator movementIterator = m_movements.find(name);
    if(movementIterator == m_movements.end())  // FIXME: should use the iterator to return the entity without having to find it twice
    {
      cout << "getMovement oops" << endl;
      return NULL;
    }
    return (*movementIterator).second;
  }

  /*!
   * Adds a behavior to the behaviors made available to entities by Level.
   *
   * Level assumes ownership of \a behavior. \a behavior is deleted if a behavior of the same name has already been added.
   *
   * \sa getMovement()
   */
  void Level::addBehavior(Behavior *behavior)
  {
    if(m_behaviors.find(behavior->name()) != m_behaviors.end())
    {
      // a behavior with the same name already exists
      delete behavior;
      return;
    }
    m_behaviors[behavior->name()] = behavior;
  }

  /*!
   * Returns the behavior stored in the Level that has the string \a name as its name.
   *
   * Returns NULL if no such behavior was found.
   *
   * \sa addBehavior()
   */
  Behavior *Level::getBehavior(const std::string &name)
  {
    if(m_behaviors.find(name) == m_behaviors.end())  // FIXME: should use the iterator to return the entity without having to find it twice
    {
      return NULL;
    }
    return m_behaviors[name];
  }

  /*!
   * Adds an entity to the entities made available by Level.
   *
   * Level assumes ownership of \a entity. \a entity is deleted if an entity of the same name has already been added.
   *
   * \sa copyEntity()
   */
  void Level::addEntity(Entity *entity)
  {
    if(m_entities.find(entity->name()) != m_entities.end())
    {
      // an entity with the same name already exists
      delete entity;
      return;
    }
    m_entities[entity->name()] = entity;
  }

  /*!
   * Returns a copy of the entity stored in Level that has the string \a name as its name.
   *
   * Level does not assume ownership of the returned entity.
   *
   * \sa addEntity()
   */
  Entity *Level::copyEntity(const std::string &name)
  {
    std::map<std::string,Entity*>::iterator entityIterator = m_entities.find(name);
    if(entityIterator == m_entities.end())
    {
      return NULL;
    }
    return new Entity(*(*entityIterator).second);  // I hate the stl's containers... what a stupid syntax
  }
}
