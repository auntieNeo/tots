#ifndef GRAPHICS_COMPONENT_H_
#define GRAPHICS_COMPONENT_H_

namespace tots {
  class Graphics;
  class GraphicsComponent {
    public:
      GraphicsComponent();
      virtual ~GraphicsComponent();

      virtual void init(Graphics *graphics) {}
      virtual void draw(const Graphics *graphics) const = 0;
  };
}

#endif
