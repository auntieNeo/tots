#include "common.h"
#include "log.h"

#include "graphics/graphics.h"
#include "graphics/triangle.h"

#include "framework/gameLoop.h"

#include <SDL.h>
#include <cstdio>
#include <unistd.h>

SDL_GLContext init_OpenGL(SDL_Window *window);

int main(int argc, char **argv)
{
  if(SDL_Init((SDL_INIT_TIMER) != 0)) {
    log_SDL_error("Unable to initialize SDL");
    return 1;
  }

  tots::GameLoop *loop = new tots::GameLoop();

  loop->run();

  delete loop;

  SDL_Quit();
  return 0;
}
