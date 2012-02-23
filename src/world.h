#ifndef WORLD_H_
#define WORLD_H_

#include <allegro5/allegro.h>
#include <string>
#include <vector>

#include "enemy.h"
#include "player.h"

namespace Tots
{ 
  class Level;

  class World
  {
    public:
      World(Level *level);
      ~World();

      void draw();
      void tick();

      void spawnEntity(int x, int y, Entity *entity);

      void setSpeed(double speed) { m_speed = speed; }

    private:
      class EntityBound
      {
        public:
          EntityBound(Entity *entity, bool isEnd) { m_entity = entity; m_isEnd = isEnd; }
          ~EntityBound() {}

          virtual int point() const = 0;
          Entity *entity() const { return m_entity; }
          bool isEnd() const { return m_isEnd; }

          bool operator <(const EntityBound &entityBound) const { return this->point() < entityBound.point(); }

        protected:
          Entity *m_entity;
          bool m_isEnd;
      };

      class EntityXStartBound : public EntityBound
      {
        public:
          EntityXStartBound(Entity *entity) : EntityBound(entity, false) {}
          ~EntityXStartBound() {}

          int point() const { return m_entity->x(); }
      };

      class EntityXEndBound : public EntityBound
      {
        public:
          EntityXEndBound(Entity *entity) : EntityBound(entity, true) {}
          ~EntityXEndBound() {}

          int point() const { return m_entity->xEndPoint(); }
      };

      class EntityYStartBound : public EntityBound
      {
        public:
          EntityYStartBound(Entity *entity) : EntityBound(entity, false) {}
          ~EntityYStartBound() {}

          int point() const { return m_entity->y(); }
      };

      class EntityYEndBound : public EntityBound
      {
        public:
          EntityYEndBound(Entity *entity) : EntityBound(entity, true) {}
          ~EntityYEndBound() {}

          int point() const { return m_entity->yEndPoint(); }
      };

      Level *m_level;
      std::vector<Entity *> m_entities;
      std::vector<Player *> m_players;
      std::list<EntityBound *> m_entityBoundsX;  // sorted bounds of the entities on the x axis
      std::list<EntityBound *> m_entityBoundsY;  // sorted bounds of the entities on the y axis
      int findEntityYIndex(Entity *entity);
      double m_speed, m_distance;

      static bool compareEntityBounds(EntityBound *a, EntityBound *b) { return a->point() < b->point(); }

      // both matricies are indexed according to the positions of the elements in m_entities
      std::vector<std::vector<unsigned int> > m_xCollisionMatrix, m_yCollisionMatrix;
  };
}

#endif
