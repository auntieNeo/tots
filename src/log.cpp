#include "log.h"

#include <cstdarg>
#include <SDL.h>

// TODO: consolidate these logging functions

void log_info(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  // TODO: log info to a file
}

void log_error(const char *msg, ...) {
  va_list ap;  // pointer to argument list

  va_start(ap, msg);  // make ap point to first arugemnt
  for(const char *p = msg; *p; p++)  /* from K&R C */
  {
    if(*p != '%') {
      putc(*p, stderr);
      continue;
    }
    switch(*++p) {
      case 'd':
        {
          int ivar = va_arg(ap, int);
          printf("%d", ivar);
        } break;
      case 'f':
        {
          double dval = va_arg(ap, double);
          printf("%f", dval);
        } break;
      case 's':
        {
          const char *sval;
          for(sval = va_arg(ap, char *); *sval; sval++)
            putchar(*sval);
        } break;
      default:
        putchar(*p);
        break;
    }
  }
  fprintf(stderr, "%s\n", msg);
  // TODO: log error to a file
}

void log_SDL_error(const char *msg)
{
  fprintf(stderr,
          "%s: %s\n",
          msg, SDL_GetError());
  // TODO: figure out SDL_Log
  SDL_ClearError();
}

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
