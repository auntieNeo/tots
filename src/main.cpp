#include "log.h"
#include "opengl.h"

#include <SDL2/SDL.h>
#include <cstdio>
#include <unistd.h>

SDL_GLContext init_OpenGL(SDL_Window *window);

int main(int argc, char **argv)
{

  if(SDL_Init((SDL_INIT_TIMER) != 0)) {
    log_SDL_error("Unable to initialize SDL");
    return 1;
  }

  fprintf(stderr, "SDL video drivers:\n");
  int numVideoDrivers = SDL_GetNumVideoDrivers();
  if(numVideoDrivers <= 0) {
    log_SDL_error("Could not find any SDL video drivers");
    return 1;
  }
  for(int i = 0; i < numVideoDrivers; ++i) {
    fprintf(stderr, "  %s\n", SDL_GetVideoDriver(i));
  }

  if(SDL_VideoInit("x11") != 0) {
    log_SDL_error("Unable to load SDL video driver");
    return 1;
  }

  if(SDL_GL_LoadLibrary(NULL) != 0) {
    log_SDL_error("Unable to load OpenGL");
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      "Typing of the Stars",
      SDL_WINDOWPOS_UNDEFINED,  // initial x position
      SDL_WINDOWPOS_UNDEFINED,  // initial y position
      640,  // width
      480,  // height
      SDL_WINDOW_OPENGL
  );

  SDL_GLContext context = init_OpenGL(window);

  // TODO: a lot of error checking
  GLuint program = glCreateProgram();
  if(program == 0)
  {
    log_OpenGL_error("Unable to create OpenGL shader program");
    return 1;
  }

  // glCreateShader
//  shader = glCreateShader();

  // TODO: glShaderSource
  // TODO: glCompileShader
  // TODO: glCreateProgram
  // TODO: glAttachShader
  // TODO: glLinkProgram
  // TODO: glUseProgram

  // draw a triangle
  // make up some vertex data

  sleep(10);

  // TODO: try atexit() to run this cleanup stuff
  SDL_GL_DeleteContext(context);

  SDL_DestroyWindow(window);

  SDL_GL_UnloadLibrary();

  SDL_Quit();
  return 0;
}
