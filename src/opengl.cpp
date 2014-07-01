#define OPENGL_CPP_
#include "opengl.h"

#include "log.h"

/*
 * Initialize all of the OpenGL API function pointers that we use. As suggested
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

  // set all of the OpenGL function pointers that we use
  glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
  glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
  glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
  glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
  glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");

  return context;
}
