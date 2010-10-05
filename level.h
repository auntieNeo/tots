#ifndef LEVEL_H_
#define LEVEL_H_

#include <list>
#include <map>
#include <string>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "path.h"
#include "sprite.h"
#include "world.h"

namespace Tots
{
  class Behavior;

  class Level
  {
    public:
      Level(const std::string &file);
      ~Level();

      void tick();
      void draw();

      void addDistanceEvent(int pixels, const std::string &luaFunction);
      void addTimedEvent(double seconds, const std::string &luaFunction);

      void changeDistance(int newDistance, World *world);
      void changeTime(double newTime, World *world);

      void addSprite(Sprite *sprite);
      Sprite *getSprite(const std::string &name);

      void addMovement(Movement *movement);
      Movement *getMovement(const std::string &name);

      void addBehavior(Behavior *behavior);
      Behavior *getBehavior(const std::string &name);

      void addEntity(Entity *entity);
      Entity *copyEntity(const std::string &name);

    private:
      struct DistanceEvent
      {
        int pixels;
        std::string luaFunction;
      };

      static bool compareDistance(DistanceEvent first, DistanceEvent second)
      {
        return first.pixels < second.pixels;
      }

      struct TimedEvent
      {
        double seconds;
        std::string luaFunction;
      };

      static bool compareTimed(TimedEvent first, TimedEvent second)
      {
        return first.seconds < second.seconds;
      }

      World *m_world;

      std::list<DistanceEvent> m_distanceEvents;
      std::list<TimedEvent> m_timedEvents;
      std::map<std::string,Sprite*> m_sprites;
      std::map<std::string,Movement*> m_movements;
      std::map<std::string,Behavior*> m_behaviors;
      std::map<std::string,Entity*> m_entities;

      lua_State *m_lua;
  };
}

#endif
