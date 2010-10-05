#ifndef QUAD_NODE_H_
#define QUAD_NODE_H_

namespace Tots
{
  class QuadNode
  {
    public:
      QuadNode(QuadNode *parent = NULL);
      ~QuadNode();

      void addEntity(Entity *entity);

    private:
      Quadnode *m_parent;
      int m_maxEntities;
  };
}

#endif
