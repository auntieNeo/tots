#include "quadNode.h"

namespace Tots
{
  QuadNode::QuadNode(QuadNode *parent)
  {
    m_parent = parent;
  }

  QuadNode::~QuadNode()
  {
  }

  void addEntity(Entity *entity)
  {
  }
}
