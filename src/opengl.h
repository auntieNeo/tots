#ifndef OPENGL_H_
#define OPENGL_H_

#ifdef OPENGL_CPP_
#define GL_EXTERN
#else
#define GL_EXTERN extern
#endif
#include <GL/gl.h>
#include <GL/glext.h>

GL_EXTERN PFNGLCREATEPROGRAMPROC glCreateProgram;

#include <SDL2/SDL.h>

SDL_GLContext init_OpenGL(SDL_Window *window);

#endif
