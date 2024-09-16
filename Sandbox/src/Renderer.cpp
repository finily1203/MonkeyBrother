#include "Renderer.h"
#include <GL/glew.h>
#include <iostream>

Renderer::Renderer() {
  
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
}

Renderer::~Renderer() {
    // Cleanup code if needed
}

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(const VertexArray& vao, const Shader& shader) const {
    shader.Bind();
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
