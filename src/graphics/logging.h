#ifndef GRAPHICS_LOGGING_H_
#define GRAPHICS_LOGGING_H_

#include "opengl.h"

int check_OpenGL_error(const char *msg);
void log_OpenGL_error(const char *msg, GLenum error = GL_NO_ERROR);

#endif
