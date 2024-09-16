#include "VertexBuffer.h"
#include <iostream>

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : m_RendererID(0) {
    glGenBuffers(1, &m_RendererID); // Use raw pointer
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    if (m_RendererID != 0) {
        glDeleteBuffers(1, &m_RendererID);
    }
}

void VertexBuffer::Bind() const {
    if (m_RendererID != 0) {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }
    else {
        std::cerr << "Invalid Renderer ID: " << m_RendererID << std::endl;
    }
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
