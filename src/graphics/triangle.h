#ifndef GRAPHICS_TRIANGLE_H_
#define GRAPHICS_TRIANGLE_H_

#include "component.h"
#include "opengl.h"

namespace tots { namespace graphics {
  class GraphicsTriangle : public GraphicsComponent {
    public:
      GraphicsTriangle();
      ~GraphicsTriangle();

      void init(Graphics *graphics);
      void draw(const Graphics *graphics) const;

    private:
      GLuint m_vaoHandle;  // TODO: make this static
  };
} }

#endif
