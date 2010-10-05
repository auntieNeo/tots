#ifndef BITMASK_H_
#define BITMASK_H_

#include <allegro5/allegro.h>

namespace Tots
{
  class Bitmask
  {
    public:
      Bitmask(ALLEGRO_BITMAP *bitmap);
      ~Bitmask();

      bool collides(const Bitmask &bitmask, int xOffset, int yOffset);

    private:
      unsigned char **m_mask;
      int m_width, m_height;
//      int m_maskHeight;
  };
}

#endif
