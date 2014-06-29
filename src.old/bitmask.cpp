#include "bitmask.h"
#include "common.h"

namespace Tots
{
  Bitmask::Bitmask(ALLEGRO_BITMAP *bitmap)
  {
    m_width = al_get_bitmap_width(bitmap);
    m_height = al_get_bitmap_height(bitmap);
    al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), ALLEGRO_LOCK_READONLY);

    // allocate the mask
    m_mask = new unsigned char *[m_width];
    m_mask[0] = new unsigned char[m_width * m_height];
    for(int x = 1; x < m_width; x++)
    {
      m_mask[x] = m_mask[0] + (x * m_height * sizeof(unsigned char));
    }
    // zero the mask
    for(int x = 0; x < m_width; x++)
    {
      for(int y = 0; y < m_height; y++)
      {
        m_mask[x][y] = 0x00;
      }
    }

    // set the mask to the bitmap's alpha
    unsigned char a, foo;
    for(int x = 0; x < m_width; x++)
    {
      for(int y = 0; y < m_height; y++)
      {
        al_unmap_rgba(al_get_pixel(bitmap, x, y), &foo, &foo, &foo, &a);
        m_mask[x][y] = a;
      }
    }

    al_unlock_bitmap(bitmap);
  }

  Bitmask::~Bitmask()
  {
  }

  bool Bitmask::collides(const Bitmask &bitmask, int xOffset, int yOffset)
  {
    // TODO: check if the bounds even overlap
    int intersectX = MAX(0, xOffset);
    int intersectX2 = MIN(m_width, bitmask.m_width + xOffset);
    int intersectY = MAX(0, yOffset / 8);
    int intersectY2 = MIN(m_height, bitmask.m_height + yOffset);

    for(int x = intersectX; x < intersectX2; x++)
    {
      for(int y = intersectY; y < intersectY2; y++)
      {
        if(m_mask[x][y] && bitmask.m_mask[x - xOffset][y - yOffset])
        {
          return true;
        }
      }
    }
    return false;
  }

  /* *** old bitwise code ***
  Bitmask::Bitmask(ALLEGRO_BITMAP *bitmap)
  {
    m_width = al_get_bitmap_width(bitmap);
    m_height = al_get_bitmap_height(bitmap);
    al_lock_bitmap(bitmap, al_get_bitmap_format(bitmap), ALLEGRO_LOCK_READONLY);

    // allocate the mask
    m_maskHeight = (m_height / 8) + ((m_height % 8) ? 1 : 0);
    m_mask = new unsigned char *[m_width];
    m_mask[0] = new unsigned char[m_width * m_maskHeight];
    for(int x = 1; x < m_width; x++)
    {
      m_mask[x] = m_mask[0] + (x * m_maskHeight * sizeof(unsigned char));
    }
    // zero the mask
    for(int x = 0; x < m_width; x++)
    {
      for(int y = 0; y < m_maskHeight: y++)
      {
        m_mask[x][y] = 0x00;
      }
    }

    // set the mask to the bitmap's alpha
    unsigned char a;
    for(int x = 0; x < m_width; x++)
    {
      for(int y = 0; y < m_height; y++)
      {
        al_unmap_rgba(al_get_pixel(bitmap, x, y), NULL, NULL, NULL, &a);
        if(a)
        {
          m_mask[x][y / 8] |= (0x01 << (y % 8));
        }
      }
    }

    al_unlock_bitmap(bitmap);
  }

  bool collides(const Bitmask &bitmask, int xOffset, int yOffset)
  {
    bool collides = false;
    // TODO: check if the bounds even overlap
    int intersectX = MAX(0, xOffset);
    int intersectX2 = MIN(m_width, bitmask.m_width + xOffset);
    int intersectY = MAX(0, yOffset / 8);
    int intersectY2 = MIN(m_maskHeight, bitmask.m_maskHeight + (yOffset / 8) + (yOffset < 0 ? -1 : 0));
    int shiftY = yOffset % 8;

    for(int x = intersectX; x < intersectX2; x++)
    {
      for(int y = intersectY; y < intersectY2; y++)
      {
        bitmask.m_mask[intersectX - xOffset]
      }
    }
  }
  */
}
