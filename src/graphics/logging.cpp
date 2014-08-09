#include "logging.h"

#include <cstdio>

int check_OpenGL_error(const char *msg)
{
  GLenum error;
  if((error = glGetError()) == GL_NO_ERROR)
    return 0;
  log_OpenGL_error(msg, error);
  return 1;
}

void log_OpenGL_error(const char *msg, GLenum error)
{
  fprintf(stderr, "%s\n", msg);
  if(error == GL_NO_ERROR)
    error = glGetError();
  do {
    fprintf(stderr,
            "  OpenGL error code: %d\n",
            error);  // FIXME: convert error code to a string
  } while ((error = glGetError()) != GL_NO_ERROR);
  // TODO: log error
}
