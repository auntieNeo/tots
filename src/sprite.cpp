#include "sprite.h"

namespace Tots
{
  // TODO: negative value in numberOfFrames "borrows" frames from another state, to avoid having to duplicate bitmaps for custom states
  Sprite::Sprite(const std::string &name, const std::string &file, int width, int height, int numberOfStates, const char *numberOfFrames, const char *framesPerSecond, int framesPerRow)
  {
    m_name = name;
    m_file = file;
    m_width = width;
    m_height = height;
    m_numberOfStates = numberOfStates;

    m_numberOfFrames = new char[numberOfStates];
    memcpy(m_numberOfFrames, numberOfFrames, numberOfStates);
    m_framesPerSecond = new char[numberOfStates];
    memcpy(m_framesPerSecond, framesPerSecond, numberOfStates);

    // load bitmap from file
    m_bitmap = al_load_bitmap(file.c_str());
    // FIXME: check for null bitmaps

    // break bitmap up into sub bitmaps
    int frameIndex = 0;
    m_frames = new ALLEGRO_BITMAP**[numberOfStates];
    m_bitmasks = new Bitmask**[numberOfStates];
    for(int i = 0; i < numberOfStates; i++)
    {
      m_frames[i] = new ALLEGRO_BITMAP*[numberOfFrames[i]];
      m_bitmasks[i] = new Bitmask*[numberOfFrames[i]];
      for(int j = 0; j < numberOfFrames[i]; j++)
      {
        m_frames[i][j] = al_create_sub_bitmap(m_bitmap, (frameIndex % framesPerRow) * width, (frameIndex / framesPerRow) * height, width, height);
        m_bitmasks[i][j] = new Bitmask(m_frames[i][j]);
        frameIndex++;
      }
    }
  }

  Sprite::~Sprite()
  {
    // FIXME: actually delete the bitmaps you idiot!
  }
}
