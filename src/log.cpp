#include "log.h"

#include "opengl.h"

#include <SDL2/SDL.h>

void log_SDL_error(const char *msg)
{
  fprintf(stderr,
          "%s: %s\n",
          msg, SDL_GetError());
  // TODO: figure out SDL_Log
  SDL_ClearError();
}

void log_OpenGL_error(const char *msg)
{
  fprintf(stderr, "%s\n", msg);
  GLenum error;
  while((error = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr,
            "  OpenGL error code: %d\n",
            error);  // FIXME: convert error code to a string
  }
  // TODO: log error
}
