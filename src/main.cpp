#include <SDL2/SDL.h>
#include <unistd.h>
#include <GL/gl.h>  // from Mesa 3D

// TODO: move the OpenGL stuff to its own file

/*
 * Initialize all of the OpenGL API function pointers that we use. As suggested
 * by the SDL documentation for SDL_GL_GetProcAddress(), we do this only after
 * activating the OpenGL context in order to maintain compatibility with
 * Windows and possibly other platforms.
 */
SDL_GLContext init_OpenGL(SDL_Window *window)
{
  if((SDL_GLContext glcontext = SDL_GL_CreateContext(window)) == NULL) {
    log_SDL_error("Unable to create OpenGL context");
    return 1;
  }
}

void log_SDL_error(const char *msg)
{
  fprintf(stderr,
          "%s: %s\n",
          msg, SDL_GetError());
  // TODO: figure out SDL_Log
  SDL_ClearError();
}

void log_OpenGL_error(const char *msg)
{
}

struct vertex {
  GL_FLOAT pos_x, pos_y;
  GL_FLOAT r, g, b, a;
};

int main(int argc, char **argv)
{

  if(SDL_Init((SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)) {
    log_SDL_error("Unable to initialize SDL");
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

  context = init_OpenGL(window);

  // TODO: a lot of error checking
  program = glCreateProgram();

  // glCreateShader
  shader = glCreateShader();

  // TODO: glShaderSource
  // TODO: glCompileShader
  // TODO: glCreateProgram
  // TODO: glAttachShader
  // TODO: glLinkProgram
  // TODO: glUseProgram

  // draw a triangle
  // make up some vertex data
  struct vertex triangle[] = {
    { 
  };
  glVertexAttribPointer(
  );

  sleep(10);

  // TODO: try atexit() to run this cleanup stuff
  SDL_GL_DeleteContext(context);

  SDL_DestroyWindow(window);

  SDL_GL_UnloadLibrary();

  SDL_Quit();
  return 0;
}
