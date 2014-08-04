#include "graphics.h"
#include "component.h"
#include "triangle.h"

#include "../common.h"
#include "../log.h"

#include <cstdlib>
#include <errno.h>
#include <string.h>

#ifdef WIN32
#define VIDEO_DRIVER "windows"
#else
#define VIDEO_DRIVER "x11"
#endif

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

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

  void Graphics::m_init(const GameState *state) {
    fprintf(stderr, "SDL video drivers:\n");
    int numVideoDrivers = SDL_GetNumVideoDrivers();
    if(numVideoDrivers <= 0) {
      log_SDL_error("Could not find any SDL video drivers");
      exit(1);  // FIXME: abort properly
    }
    for(int i = 0; i < numVideoDrivers; ++i) {
      fprintf(stderr, "  %s\n", SDL_GetVideoDriver(i));
    }

    if(SDL_VideoInit(VIDEO_DRIVER) != 0) {
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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // enable depth testing
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);

    // find all the shaders
    // directory containing all vertex (.vert), fragment (.frag), etc. shaders
    // TODO: move this into a routine
    size_t numVertShaderPaths, numFragShaderPaths;
    char **vertShaderPaths = m_findShaders("./src/shaders", ".vert", &numVertShaderPaths);
    for(size_t i = 0; i < numVertShaderPaths; ++i)
      printf("found vert shader: %s\n", vertShaderPaths[i]);
    char **fragShaderPaths = m_findShaders("./src/shaders", ".frag", &numFragShaderPaths);
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
      delete[] fragShaderPaths[i];
    delete[] fragShaderPaths;
    for(size_t i = 0; i < numVertShaderPaths; ++i)
      delete[] vertShaderPaths[i];
    delete[] vertShaderPaths;

    // TODO: spawn this properly
    GraphicsTriangle *triangle = new GraphicsTriangle();
    addComponent(triangle);
  }

  void Graphics::m_close(const GameState *state) {
    free(m_fragShaders);
    free(m_vertShaders);

    // TODO: try atexit() to run this cleanup stuff
    SDL_GL_DeleteContext(m_context);

    SDL_DestroyWindow(m_window);

    SDL_GL_UnloadLibrary();

    SDL_VideoQuit();

    // TODO: delete components
  }

  void Graphics::addComponent(GraphicsComponent *component) {
    assert(m_numComponents < MAX_COMPONENTS);
    m_components[m_numComponents++] = component;
    component->init(this);
  }


#define MAX_SHADERS 256
  char **Graphics::m_findShaders(const char *dirp, const char *ext, size_t *numShaders) {
    *numShaders = 0;
    char **result = new char*[MAX_SHADERS];
    if(!fs::exists(dirp) || !fs::is_directory(dirp)) {
      log_error("Could not open shader directory: %s", dirp);
      // FIXME: abort properly here
    }
    for(fs::directory_iterator i(dirp); i != fs::directory_iterator(); ++i) {
      if(strcmp(i->path().extension().string().c_str(), ext) == 0) {
        if(*numShaders >= MAX_SHADERS) {
          log_error("Tried to load too many shaders in: %s", dirp);
          // FIXME: abort properly here
        }
        printf("found: %s\n", i->path().c_str());
        size_t shaderIndex = (*numShaders)++;
        size_t pathLength = strlen(i->path().c_str()) + 1;
        result[shaderIndex] = new char[pathLength];
        strncpy(result[shaderIndex], i->path().c_str(), pathLength);
      }
    }
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
    glShaderSource(shader, 1, (const GLchar **)&source, NULL);
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

  void Graphics::m_update(const GameState *state) {
    // TODO: set graphics context
    // TODO: buffering, etc.
    glClear(GL_COLOR_BUFFER_BIT);
    for(size_t i = 0; i < m_numComponents; ++i) {
      m_components[i]->draw(this);
    }

    // swap the buffer to actually display it
    SDL_GL_SwapWindow(m_window);
  }
}
