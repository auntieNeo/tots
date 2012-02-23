#include <algorithm>
#include <allegro5/allegro_primitives.h>
#include <cassert>

#include "behavior.h"
#include "common.h"
#include "level.h"
#include "playerState.h"
#include "world.h"

#include <iostream>
using namespace std;

namespace Tots
{ 
  /*!
   * \class World
   * World is responsible for spawned entities and their interactions with the player. It calls tick and collision methods for all entities, and destroys them when appropriate.
   *
   * World is largely controlled by Level and its lua scripts, which configure and spawn the entities.
   */
  World::World(Level *level)
  {
    m_level = level;
    m_speed = 50;
    m_distance = 0;
    for(size_t i = 0; i < sizeof(unsigned int) * 8; i++)
    {
      m_xCollisionMatrix.push_back(std::vector<unsigned int>(1, 0));
      m_yCollisionMatrix.push_back(std::vector<unsigned int>(1, 0));
    }
  }

  World::~World()
  {
    delete m_level;
    for(size_t i = 0; i < m_entities.size(); i++)
    {
      delete m_entities[i];
    }
    for(size_t i = 0; i < m_players.size(); i++)
    {
      delete m_players[i];
    }
  }

  void World::draw()
  {
    al_clear_to_color(al_map_rgb(0x00, 0x00, 0x00));
    for(size_t i = 0; i < m_entities.size(); i++)
    {
      m_entities[i]->draw(m_entities[i]->x(), m_entities[i]->y() + m_distance);
    }
  }

  void World::tick()
  {
    static double m_time = 0;
    m_time += 1.0 / FRAMES_PER_SECOND;
    static long int m_tick = 0;
    m_tick++;

    // move everything based on m_speed
    m_distance += m_speed / FRAMES_PER_SECOND;
    for(size_t i = 0; i < m_players.size(); i++)
    {
      m_players[i]->setY(m_players[i]->y() - m_speed / FRAMES_PER_SECOND);
    }

    if(m_players.size() == 0)
    {
      // FIXME: this is not how player should be initilized!
      PlayerState *playerState = new PlayerState("player01", MAX_VELOCITY, ACCELERATION, FRICTION, ALLEGRO_KEY_FULLSTOP, ALLEGRO_KEY_E, ALLEGRO_KEY_O, ALLEGRO_KEY_U);
      m_level->addBehaviorState(playerState);
      Behavior *playerBehavior = new Behavior("player01", Behavior::FIRST_CUSTOM_STATE);
      playerBehavior->addState(Behavior::IDLE, m_level->getBehaviorState("player01"));
      m_level->addBehavior(playerBehavior);
      Player *player = new Player("player", Entity::PLAYER, m_level->getSprite("player"), m_level->getBehavior("player01"));
      m_players.push_back(player);
      player->setIndex(m_entities.size());
      m_entities.push_back(player);
      m_entityBoundsX.push_back(new EntityXStartBound(player));
      m_entityBoundsX.push_back(new EntityXEndBound(player));
      m_entityBoundsY.push_back(new EntityYStartBound(player));
      m_entityBoundsY.push_back(new EntityYEndBound(player));
      m_entityBoundsX.sort(compareEntityBounds);
      m_entityBoundsY.sort(compareEntityBounds);
    }

    // tick entities
    for(size_t i = 0; i < m_entities.size(); i++)
    {
      m_entities[i]->tick();
    }

    m_level->changeDistance(42, this);  // TODO: implement distance events
    m_level->changeTime(m_time, this);

    // collision detection
    // sort the bounds of the entities
    // TODO: replace this with insertion sort
    m_entityBoundsX.sort(compareEntityBounds);
    m_entityBoundsY.sort(compareEntityBounds);
    // x axis collision detection
    std::list<Entity *> activeEntities;
    for(std::list<EntityBound *>::iterator i = m_entityBoundsX.begin(); i != m_entityBoundsX.end(); i++)
    {
      if((*i)->isEnd())
      {
        // find entity in activeEntities and remove it
        for(std::list<Entity *>::iterator j = activeEntities.begin(); ; j++)
        {
          assert(j != activeEntities.end());
          if(*j == (*i)->entity())
          {
            activeEntities.erase(j);
            break;
          }
        }
      }
      else
      {
        // entity collides with every entity in activeEntities
        for(std::list<Entity *>::iterator j = activeEntities.begin(); j != activeEntities.end(); j++)
        {
          // exclude certain entity types
          if((*i)->entity()->type() == (*j)->type())
          {
            continue;
          }
          // add the collision to the collision matrix
          size_t iIndex, jIndex;
          if((*i)->entity()->index() < (*j)->index())
          {
            iIndex = (*i)->entity()->index();
            jIndex = (*j)->index();
          }
          else
          {
            iIndex = (*j)->index();
            jIndex = (*i)->entity()->index();
          }
          m_xCollisionMatrix[iIndex][jIndex / (sizeof(unsigned int) * 8)] |= 1 << jIndex % (sizeof(unsigned int) * 8);
        }
        // add entity to activeEntities
        activeEntities.push_back((*i)->entity());
      }
    }
    // y axis collision detection
    assert(activeEntities.empty());
    for(std::list<EntityBound *>::iterator i = m_entityBoundsY.begin(); i != m_entityBoundsY.end(); i++)
    {
      if((*i)->isEnd())
      {
        // find entity in activeEntities and remove it
        for(std::list<Entity *>::iterator j = activeEntities.begin(); ; j++)
        {
          assert(j != activeEntities.end());
          if(*j == (*i)->entity())
          {
            activeEntities.erase(j);
            break;
          }
        }
      }
      else
      {
        // entity collides with every entity in activeEntities
        for(std::list<Entity *>::iterator j = activeEntities.begin(); j != activeEntities.end(); j++)
        {
          // exclude certain entity types
          if((*i)->entity()->type() == (*j)->type())
          {
            continue;
          }
          // add the collision to the collision matrix
          size_t iIndex, jIndex;
          if((*i)->entity()->index() < (*j)->index())
          {
            iIndex = (*i)->entity()->index();
            jIndex = (*j)->index();
          }
          else
          {
            iIndex = (*j)->index();
            jIndex = (*i)->entity()->index();
          }
          m_yCollisionMatrix[iIndex][jIndex / (sizeof(unsigned int) * 8)] |= 1 << jIndex % (sizeof(unsigned int) * 8);
        }
        // add entity to activeEntities
        activeEntities.push_back((*i)->entity());
      }
    }
    // read the collision matricies
    for(size_t i = 0; i < m_xCollisionMatrix.size(); i++)
    {
      for(size_t j = i / (sizeof(unsigned int) * 8); j < m_xCollisionMatrix[i].size(); j++)
      {
        if(unsigned int collision = m_xCollisionMatrix[i][j] & m_yCollisionMatrix[i][j])
        {
          for(unsigned char shift = 0; shift < sizeof(unsigned int) * 8; shift++)
          {
            if(collision & (1 << shift))
            {
              if(m_entities[i]->collides(m_entities[j * (sizeof(unsigned int) * 8) + shift]))  // narrow phase collision detection
              {
                m_entities[i]->handleCollision(m_entities[j * (sizeof(unsigned int) * 8) + shift]);
                m_entities[j * (sizeof(unsigned int) * 8) + shift]->handleCollision(m_entities[i]);
              }
            }
          }
        }
        // clear the matricies
        m_xCollisionMatrix[i][j] = 0;
        m_yCollisionMatrix[i][j] = 0;
      }
    }

    // TODO: iterate through m_entities to find entities marked for destruction, and if found, sort the two bounds lists again, binary search them, and remove both copies
    for(size_t i = 0; i < m_entities.size(); i++)
    {

    }
  }

  /*!
   * Spawns \a entity to the world at position \a x, \a y.
   *
   * World assumes ownership of \a entity.
   */
  void World::spawnEntity(int x, int y, Entity *entity)
  {
    entity->setX(x);
    entity->setY(y - m_distance);
    entity->setCurrentState(Behavior::IDLE);  // FIXME: should be SPAWNED
    // TODO: set entity index to a freed index
    entity->setIndex(m_entities.size());
    m_entities.push_back(entity);
    m_entityBoundsX.push_back(new EntityXStartBound(entity));
    m_entityBoundsX.push_back(new EntityXEndBound(entity));
    m_entityBoundsY.push_back(new EntityYStartBound(entity));
    m_entityBoundsY.push_back(new EntityYEndBound(entity));
    m_entityBoundsX.sort(compareEntityBounds);
    m_entityBoundsY.sort(compareEntityBounds);

    // grow collision matricies
    if(m_entities.size() > m_xCollisionMatrix.size())
    {
      // grow down
      for(size_t i = 0; i < m_xCollisionMatrix.size(); i++)
      {
        m_xCollisionMatrix[i].push_back(0);
        m_yCollisionMatrix[i].push_back(0);
      }
      // grow sideways
      for(size_t i = 0; i < sizeof(unsigned int) * 8; i++)
      {
        m_xCollisionMatrix.push_back(std::vector<unsigned int>(m_xCollisionMatrix[0].size(), 0));
        m_yCollisionMatrix.push_back(std::vector<unsigned int>(m_yCollisionMatrix[0].size(), 0));
      }
    }
  }
}
