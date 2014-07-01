#include "common.h"
#include "log.h"

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

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  if(vertexShader == 0) {
    log_OpenGL_error("Unable to create OpenGL vertex shader");
    return 1;
  }

  // FIXME: move this into some routines
  const char *file_path = "src/shaders/simple.vert";
  FILE *f = fopen(file_path, "r");
  if(f == NULL) {
    log_error("could not open file %s\n", file_path);
    return 1;
  }
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f) + 1;
  assert(fsize > 0);
  fseek(f, 0, SEEK_SET);
  GLchar *source = (GLchar *)malloc(fsize);
  if(source == NULL) {
    log_error("could not malloc");
    exit(1);
  }
  size_t num_read = fread(source, 1, fsize, f);
  assert(num_read == (unsigned long)fsize - 1);
  glShaderSource(vertexShader, 1, &source, NULL);
  if(check_OpenGL_error("Could not set shader source"))
    exit(1);
  free(source);
  source = NULL;

  fprintf(stderr, "Compiling \"%s\"...", file_path);
  fflush(stderr);
  glCompileShader(vertexShader);
  fprintf(stderr, " done.\n");
  GLint compileStatus, infoLogLength;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
  GLchar * infoLog = (GLchar *)malloc(infoLogLength);
  glGetShaderInfoLog(vertexShader, infoLogLength, NULL, infoLog);
  log_info(infoLog);  // FIXME: I must change this if I ever update log_info to use printf format
  free(infoLog);
  infoLog = NULL;
  if(compileStatus == GL_FALSE) {
    log_OpenGL_error("error compiling vertex shader");
    exit(1);
  }

  GLuint program = glCreateProgram();
  if(program == 0) {
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
