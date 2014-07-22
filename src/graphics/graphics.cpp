#include "graphics.h"
#include "component.h"

#include "../common.h"
#include "../log.h"

#include <cstdlib>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <cstdio>

#define MAX_COMPONENTS 128

namespace tots {
  Graphics::Graphics() {
    m_window = NULL;
    m_vertShaders = m_fragShaders = NULL;
    m_numVertShaders = m_numFragShaders = 0;
    m_program = 0;
    m_components = static_cast<GraphicsComponent **>(malloc(sizeof(GraphicsComponent *) * MAX_COMPONENTS));
    m_numComponents = 0;
  }

  Graphics::~Graphics() {
    m_numVertShaders = m_numFragShaders = 0;
    for(size_t i = 0; i < m_numComponents; ++i) {
      delete m_components[i];
    }
    delete m_components;
    m_components = NULL;
    // TODO: call this->close() safely in destructor
  }

  void Graphics::init() {
    fprintf(stderr, "SDL video drivers:\n");
    int numVideoDrivers = SDL_GetNumVideoDrivers();
    if(numVideoDrivers <= 0) {
      log_SDL_error("Could not find any SDL video drivers");
      exit(1);  // FIXME: abort properly
    }
    for(int i = 0; i < numVideoDrivers; ++i) {
      fprintf(stderr, "  %s\n", SDL_GetVideoDriver(i));
    }

    if(SDL_VideoInit("x11") != 0) {
      log_SDL_error("Unable to load SDL video driver");
      exit(1);  // FIXME: abort properly
    }

    if(SDL_GL_LoadLibrary(NULL) != 0) {
      log_SDL_error("Unable to load OpenGL");
      exit(1);
    }

    m_window = SDL_CreateWindow(
        "Typing of the Stars",
        SDL_WINDOWPOS_UNDEFINED,  // initial x position
        SDL_WINDOWPOS_UNDEFINED,  // initial y position
        640,  // width
        480,  // height
        SDL_WINDOW_OPENGL
    );

    m_context = init_OpenGL(m_window);

    // enable smooth shading
    glShadeModel(GL_SMOOTH);

    // set the background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // enable depth testing
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);

    // find all the shaders
    // directory containing all vertex (.vert), fragment (.frag), etc. shaders
    // TODO: move this into a routine
    size_t numVertShaderPaths, numFragShaderPaths;
    char **vertShaderPaths = m_findShaders("./src/shaders", "vert", &numVertShaderPaths);
    for(size_t i = 0; i < numVertShaderPaths; ++i)
      printf("found vert shader: %s\n", vertShaderPaths[i]);
    char **fragShaderPaths = m_findShaders("./src/shaders", "frag", &numFragShaderPaths);
    for(size_t i = 0; i < numFragShaderPaths; ++i)
      printf("found frag shader: %s\n", fragShaderPaths[i]);

    // allocate memory for array of shader refs
    m_vertShaders = (GLuint *)malloc(sizeof(GLuint) * numVertShaderPaths);
    m_fragShaders = (GLuint *)malloc(sizeof(GLuint) * numFragShaderPaths);

    // TODO: load shader code into OpenGL
    for(size_t i = 0; i < numVertShaderPaths; ++i)
      m_loadShader(vertShaderPaths[i], GL_VERTEX_SHADER);
    for(size_t i = 0; i < numFragShaderPaths; ++i)
      m_loadShader(fragShaderPaths[i], GL_FRAGMENT_SHADER);

    // create the shader program
    m_program = glCreateProgram();
    if(m_program == 0) {
      log_OpenGL_error("Unable to create OpenGL shader program");
      exit(1);  // FIXME: abort properly
    }

    // attach shaders to shader program
    for(size_t i = 0; i < m_numVertShaders; ++i) {
      fprintf(stderr, "Attaching vertex shader...\n");
      glAttachShader(m_program, m_vertShaders[i]);
      if(check_OpenGL_error("Could not attach vertex shader"))
        exit(1);  // FIXME: abort properly
    }
    for(size_t i = 0; i < m_numFragShaders; ++i) {
      fprintf(stderr, "Attaching fragment shader...\n");
      glAttachShader(m_program, m_fragShaders[i]);
      if(check_OpenGL_error("Could not attach fragment shader"))
        exit(1);  // FIXME: abort properly
    }

    // link the shader program
    fprintf(stderr, "Linking shader program...\n");
    glLinkProgram(m_program);
    GLint linkStatus, infoLogLength;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linkStatus);
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = (GLchar *)malloc(infoLogLength * sizeof(GLchar));
    glGetProgramInfoLog(m_program, infoLogLength, NULL, infoLog);
    log_info(infoLog);
    free(infoLog);
    infoLog = NULL;
    if(linkStatus == GL_FALSE) {
      log_OpenGL_error("error linking shader program");
      exit(1);  // FIXME: abort properly
    }

    // install shader program in current rendering context
    fprintf(stderr, "Installing shader program...\n");
    glUseProgram(m_program);
    if(check_OpenGL_error("Could not use shader program"))
      exit(1);  // FIXME: abort properly

    for(size_t i = 0; i < numFragShaderPaths; ++i)
      free(fragShaderPaths[i]);  // allocated via malloc(3) as per scandir(3) man page
    for(size_t i = 0; i < numVertShaderPaths; ++i)
      free(vertShaderPaths[i]);  // allocated via malloc(3) as per scandir(3) man page
  }

  void Graphics::close() {
    free(m_fragShaders);
    free(m_vertShaders);

    // TODO: try atexit() to run this cleanup stuff
    SDL_GL_DeleteContext(m_context);

    SDL_DestroyWindow(m_window);

    SDL_GL_UnloadLibrary();

    SDL_VideoQuit();
  }

  void Graphics::m_update(const GameState *state) {
    // TODO: set graphics context
    // TODO: buffering, etc.
    printf("In graphics thread.\n");
    glClear(0xFFFF);
    for(size_t i = 0; i < m_numComponents; ++i) {
      printf("drawing...\n");
      m_components[i]->draw(this);
    }

    // swap the buffer to actually display it
    SDL_GL_SwapWindow(m_window);
  }

  void Graphics::addComponent(GraphicsComponent *component) {
    assert(m_numComponents < MAX_COMPONENTS);
    m_components[m_numComponents++] = component;
    component->init(this);
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
      // join the filename back to the dirname
      char *relative_path = (char *)malloc((strlen(dirp) + strlen(de[i]->d_name) + 2 /* slash and \0 */) * sizeof(char));  // FIXME: use MAX_PATH
      sprintf(relative_path, "%s/%s", dirp, de[i]->d_name);
      // get the absolute path
      result[i] = realpath(relative_path, NULL);  // allocated via malloc(3) as per realpath(3) man page
      free(relative_path);
//      free(de[i]->d_name);  // FIXME: not a valid pointer, says free()  // allocated via malloc(3) as per scandir(3) man page
    }
    free(de);  // allocated via malloc(3) as per scandir(3) man page

    return result;
  }

  GLuint Graphics::m_loadShader(const char *sourcePath, GLenum shaderType) {
    GLuint **shaders;
    size_t *numShaders;

    switch(shaderType) {
      case GL_VERTEX_SHADER:
        shaders = &m_vertShaders;
        numShaders = &m_numVertShaders;
        break;
      case GL_FRAGMENT_SHADER:
        shaders = &m_fragShaders;
        numShaders = &m_numFragShaders;
        break;
      default:
        exit(1);  // FIXME: abort properly
    }

    // create the shader
    GLuint shader = glCreateShader(shaderType);
    if(shader == 0) {
      log_OpenGL_error("Unable to create OpenGL shader");
      exit(1);  // FIXME: abort properly
    }

    // load the shader source from file
    FILE *f = fopen(sourcePath, "r");
    if(f == NULL) {
      log_error("could not open file %s\n", sourcePath);
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
    glShaderSource(shader, 1, &source, NULL);
    if(check_OpenGL_error("Could not set shader source"))
      exit(1);  // FIXME: abort properly
    free(source);
    source = NULL;

    // compile the shader
    fprintf(stderr, "Compiling \"%s\"...", sourcePath);
    fflush(stderr);
    glCompileShader(shader);
    fprintf(stderr, " done.\n");
    GLint compileStatus, infoLogLength;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = (GLchar *)malloc(infoLogLength * sizeof(GLchar));
    glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
    log_info(infoLog);  // FIXME: I must change this if I ever update log_info to use printf format
    free(infoLog);
    infoLog = NULL;
    if(compileStatus == GL_FALSE) {
      log_OpenGL_error("error compiling vertex shader");
      exit(1);  // FIXME: abort properly
    }

    // add shader to list
    (*shaders)[(*numShaders)++] = shader;

    return shader;
  }
}
