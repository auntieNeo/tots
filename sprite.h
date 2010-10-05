#ifndef SPRITE_H_
#define SPRITE_H_

#include <allegro5/allegro.h>
#include <string>

#include "entity.h"
#include "bitmask.h"

namespace Tots
{
  class Sprite
  {
    public:
      Sprite(const std::string &name, const std::string &file, int width, int height, int numberOfStates, const char *numberOfFrames, const char *framesPerSecond, int framesPerRow = 10);
      ~Sprite();

      void drawSprite(ALLEGRO_BITMAP *bitmap, int x, int y, int state, int frame);

      std::string name() { return m_name; }
      std::string file() { return m_file; }
      int width() { return m_width; }
      int height() { return m_height; }

      int numberOfStates() { return m_numberOfStates; }
      int numberOfFrames(Entity::State state) { return m_numberOfFrames[state]; }  // TODO: bounds checking
      int framesPerSecond(Entity::State state) { return m_framesPerSecond[state]; }
      ALLEGRO_BITMAP *bitmap() { return m_bitmap; }
      ALLEGRO_BITMAP *frame(Entity::State state, int frame) { return m_frames[state][frame]; }
      Bitmask *bitmask(Entity::State state, int frame) { return m_bitmasks[state][frame]; }

    private:
      std::string m_name, m_file;
      int m_width, m_height;
      int m_numberOfStates;
      char *m_numberOfFrames, *m_framesPerSecond;
      ALLEGRO_BITMAP *m_bitmap;
      ALLEGRO_BITMAP ***m_frames;
      Bitmask ***m_bitmasks;
  };
}

#endif
