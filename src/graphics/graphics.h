#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "opengl.h"

#include <cstddef>

namespace tots {
  class GraphicsComponent;
  class Graphics {
    public:
      Graphics();
      ~Graphics();

      void init();
      void close();
      void update() const;

      void addComponent(GraphicsComponent *component);

    private:
      SDL_Window *m_window;
      SDL_GLContext m_context;

      GLuint *m_vertShaders, *m_fragShaders;
      size_t m_numVertShaders, m_numFragShaders;

      GLuint m_program;

      GraphicsComponent **m_components;  // FIXME: use a hash/map/set here
      size_t m_numComponents;

      static char **m_findShaders(const char *dirp, const char *ext, size_t *numShaders);

      GLuint m_loadShader(const char *sourcePath, GLenum shaderType);
  };
}

#endif
