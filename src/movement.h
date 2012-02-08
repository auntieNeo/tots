#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include <string>

#include "common.h"

namespace Tots
{
  class Entity;

  class Movement
  {
    public:
      Movement(const std::string &name);
      virtual ~Movement();

      std::string name() { return m_name; }

      virtual void tick(Entity *entity) = 0;

    private:
      std::string m_name;
  };
}
#endif
