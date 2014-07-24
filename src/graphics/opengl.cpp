#define OPENGL_CPP_
#include "opengl.h"

#include "../log.h"

/*
 * Initialize all of the OpenGL API function pointers using GLEW. As suggested
 * by the SDL documentation for SDL_GL_GetProcAddress(), we do this only after
 * activating the OpenGL context in order to maintain compatibility with
 * Windows and possibly other platforms.
 */
SDL_GLContext init_OpenGL(SDL_Window *window)
{
  SDL_GLContext context;
  if((context = SDL_GL_CreateContext(window)) == NULL) {
    log_SDL_error("Unable to create OpenGL context");
    return NULL;
  }

  GLenum error = glewInit();
  if(error != GLEW_OK) {
    log_error("GLEW Error: %s\n", glewGetErrorString(error));
  }
  //log_info(glewGetString(GLEW_VERSION));  // FIXME: support format string in log_info

  return context;
}
