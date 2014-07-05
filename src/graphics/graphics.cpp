#include "graphics.h"

#include "../common.h"

#include <cstdlib>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <cstdio>

namespace tots {
  Graphics::Graphics() {
    m_vertShaderPaths = m_fragShaderPaths = NULL;
    m_numVertShaders = m_numFragShaders = 0;
  }

  Graphics::~Graphics() {
  }

  void Graphics::init() {
    // find all the shaders
    // directory containing all vertex (.vert), fragment (.frag), etc. shaders
    m_vertShaderPaths = m_findShaders("./src/shaders", "vert", &m_numVertShaders);
    for(size_t i = 0; i < m_numVertShaders; ++i)
      printf("found vert shader: %s\n", m_vertShaderPaths[i]);
    m_fragShaderPaths = m_findShaders("./src/shaders", "frag", &m_numFragShaders);
    for(size_t i = 0; i < m_numFragShaders; ++i)
      printf("found frag shader: %s\n", m_fragShaderPaths[i]);

    // TODO: load shader code into OpenGL

    for(size_t i = 0; i < m_numFragShaders; ++i)
      free(m_fragShaderPaths);
    for(size_t i = 0; i < m_numVertShaders; ++i)
      free(m_vertShaderPaths);
  }

  void Graphics::close() {
  }

  void Graphics::update() const {
  }

  const char *filter_ext;
  int filterExt(const struct dirent *de) {
    assert(filter_ext != NULL);
    size_t ext_len = strlen(filter_ext);
    const char *match;
    if((match = strstr(de->d_name, filter_ext)) != NULL)
      if(match[ext_len] == '\0')
        return 1;
    return 0;
  }

  char **Graphics::m_findShaders(const char *dirp, const char *ext, size_t *numShaders) {
    struct dirent **de;
    assert(filter_ext == NULL);
    filter_ext = ext;
    int n;
    if((n = scandir(dirp, &de, filterExt, alphasort)) == -1) {  // FIXME: Valgrind says this leaks
      printf("Graphics::m_findShaders() failed: %s\n", strerror(errno));
      *numShaders = 0;
      exit(1);  // FIXME: abort properly here
    }
    *numShaders = n;
    filter_ext = NULL;
    char **result = static_cast<char **>(malloc(sizeof(char *) * (*numShaders)));  // FIXME: check this malloc
    for(size_t i = 0; i < *numShaders; ++i) {
      result[i] = de[i]->d_name;  // allocated via malloc(3) as per scandir(3) man page
    }
    free(de);  // allocated via malloc(3) as per scandir(3) man page

    return result;
  }
}
