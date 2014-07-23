#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "opengl.h"
#include "../framework/subsystem.h"

#include <cstddef>

namespace tots {
  class GameState;
  class GraphicsComponent;
  class Graphics : public Subsystem {
    public:
      Graphics();
      ~Graphics();

      void close();

      void addComponent(GraphicsComponent *component);
      Subsystem::Hints hints() const { return Subsystem::HOG_THREAD; }

      const char *name() const { return "Graphics Subsystem"; }

    protected:
      void m_init(const GameState *state);
      void m_update(const GameState *state);
      void m_close(const GameState *state);

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
