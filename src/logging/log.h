#ifndef LOG_H_
#define LOG_H_

// FIXME: this might be a bad dependency
#include "../graphics/opengl.h"

// TODO: make log_critical_error() function
void log_info(const char *msg);
void log_error(const char *msg, ...);
void log_SDL_error(const char *msg);

#endif
