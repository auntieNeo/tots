#include "triangle.h"

#include "opengl.h"

namespace tots {
  GraphicsTriangle::GraphicsTriangle() {
  }

  GraphicsTriangle::~GraphicsTriangle() {
  }

  void GraphicsTriangle::init(Graphics *graphics) {
    GraphicsComponent::init(graphics);
    float positionData[] = {
      -0.8f, -0.8f, 0.0f,
       0.8f, -0.8f, 0.0f,
       0.0f,  0.8f, 0.0f };
    float colorData[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f };
    // create the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];
    // populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);
    // populate the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    // create and set-up the vertex array object
    glGenVertexArrays(1, &m_vaoHandle);
    glBindVertexArray(m_vaoHandle);
    // enable the vertex attribute arrays
    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex color
    // map index 0 to the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
    // map index 1 to the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
  }

  void GraphicsTriangle::draw(const Graphics *graphics) const {
    glBindVertexArray(m_vaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }
}
