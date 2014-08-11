#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "opengl.h"
#include "../framework/subsystem.h"

#include <cstddef>

namespace tots {
  namespace framework {
    class GameState;
  }
  namespace graphics {
    class GraphicsComponent;
    class Graphics : public tots::framework::Subsystem {
      public:
        Graphics();
        ~Graphics();

        void addComponent(GraphicsComponent *component);
        Subsystem::Hints hints() const { return /* Hints::HOG_THREAD | */ Hints::UPDATE_EACH_FRAME; }

        const char *name() const { return "Graphics"; }
        utility::StringId address() const { return ADDRESS("Graphics"); }
        int32_t updatePeriod() const { return 1; }

      protected:
        void m_init();
        void m_update();
        void m_close();

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
}

#endif
