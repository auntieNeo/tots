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
  glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
  glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
  glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
  glCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
  glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)SDL_GL_GetProcAddress("glGetProgramInfoLog");
  glGetProgramiv = (PFNGLGETPROGRAMIVPROC)SDL_GL_GetProcAddress("glGetProgramiv");
  glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
  glGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
  glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
  glShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
  glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");

  return context;
}
