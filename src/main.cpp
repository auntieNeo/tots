#include "common.h"
#include "logging/log.h"

#include "graphics/graphics.h"
#include "graphics/triangle.h"

#include "framework/gameLoop.h"

#include <SDL.h>
#include <cstdio>

#define MAX_SUBSYSTEMS 16

int main(int argc, char **argv)
{
  tots::framework::Subsystem **subsystems = new tots::framework::Subsystem*[MAX_SUBSYSTEMS];
  size_t numSubsystems = 0;

  if(SDL_Init((SDL_INIT_TIMER) != 0)) {
    log_SDL_error("Unable to initialize SDL");
    return 1;
  }

  tots::graphics::Graphics *gfx = new tots::graphics::Graphics();
  assert(numSubsystems < MAX_SUBSYSTEMS);
  subsystems[numSubsystems++] = static_cast<tots::framework::Subsystem *>(gfx);

  tots::framework::GameLoop *loop = new tots::framework::GameLoop(subsystems, numSubsystems);

  loop->run();

  delete loop;

  delete gfx;

  SDL_Quit();

  delete[] subsystems;

  return 0;
}
