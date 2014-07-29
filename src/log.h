#ifndef LOG_H_
#define LOG_H_

#include "graphics/opengl.h"

// TODO: make log_critical_error() function
void log_info(const char *msg);
void log_error(const char *msg, ...);
void log_SDL_error(const char *msg);
int check_OpenGL_error(const char *msg);
void log_OpenGL_error(const char *msg, GLenum error = GL_NO_ERROR);

#endif
