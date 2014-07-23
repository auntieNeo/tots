#include "common.h"
#include "log.h"

#include "graphics/graphics.h"
#include "graphics/triangle.h"

#include "framework/gameLoop.h"

#include <SDL2/SDL.h>
#include <cstdio>
#include <unistd.h>

#define MAX_SUBSYSTEMS 16

int main(int argc, char **argv)
{
  tots::Subsystem **subsystems = new tots::Subsystem*[MAX_SUBSYSTEMS];
  size_t numSubsystems = 0;

  if(SDL_Init((SDL_INIT_TIMER) != 0)) {
    log_SDL_error("Unable to initialize SDL");
    return 1;
  }

  tots::Graphics *gfx = new tots::Graphics();
  assert(numSubsystems < MAX_SUBSYSTEMS);
  subsystems[numSubsystems++] = static_cast<tots::Subsystem *>(gfx);

  tots::GameLoop *loop = new tots::GameLoop(subsystems, numSubsystems);

  loop->run();

  delete loop;

  delete gfx;

  SDL_Quit();

  delete[] subsystems;

  return 0;
}
