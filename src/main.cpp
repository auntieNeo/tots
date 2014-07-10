#include "common.h"
#include "log.h"

#include "graphics/graphics.h"
#include "graphics/triangle.h"

#include <SDL2/SDL.h>
#include <cstdio>
#include <unistd.h>

SDL_GLContext init_OpenGL(SDL_Window *window);

int main(int argc, char **argv)
{

  if(SDL_Init((SDL_INIT_TIMER) != 0)) {
    log_SDL_error("Unable to initialize SDL");
    return 1;
  }

  tots::Graphics gfx = tots::Graphics();

  gfx.init();

  tots::GraphicsTriangle *triangle = new tots::GraphicsTriangle();
  gfx.addComponent(triangle);

  gfx.update();

  sleep(10);

  gfx.close();

  SDL_Quit();
  return 0;
}
