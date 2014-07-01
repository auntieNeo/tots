#include "log.h"

#include <SDL2/SDL.h>

void log_SDL_error(const char *msg)
{
  fprintf(stderr,
          "%s: %s\n",
          msg, SDL_GetError());
  // TODO: figure out SDL_Log
  SDL_ClearError();
}
