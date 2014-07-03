#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <cstddef>

namespace tots {
  class Graphics {
    public:
      Graphics();
      ~Graphics();

      void init();
      void close();
      void update() const;

    private:
      char **m_vertShaderPaths, **m_fragShaderPaths;
      size_t m_numVertShaders, m_numFragShaders;

      static char **m_findShaders(const char *dirp, const char *ext, size_t *numShaders);
  };
}

#endif
