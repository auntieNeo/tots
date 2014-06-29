#include <SDL2/SDL.h>
#include <unistd.h>

int main(int argc, char **argv)
{

  if(SDL_Init((SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0))
  {
    fprintf(stderr,
            "Unable to initialize SDL: %s\n",
            SDL_GetError());
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

  sleep(10);

  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
