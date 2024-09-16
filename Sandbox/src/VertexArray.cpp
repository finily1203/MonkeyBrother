#include "VertexArray.h"
#include <iostream>

// Constructor: Initialize member variable
VertexArray::VertexArray() : m_RendererID(0) {
}

// Destructor: Delete the VAO
VertexArray::~VertexArray() {
    if (m_RendererID != 0) {
        glDeleteVertexArrays(1, &m_RendererID); // Delete the VAO

    }
}

// Bind the VAO
void VertexArray::Bind() const {
    if (m_RendererID != 0) {
        glBindVertexArray(m_RendererID); // Bind the VAO
    }
}

// Unbind the VAO
void VertexArray::Unbind() const {
    glBindVertexArray(0); // Unbind the VAO
}

// Explicitly initialize the VAO
void VertexArray::Initialize() {
    if (m_RendererID == 0) {
        glGenVertexArrays(1, &m_RendererID); // Generate the VAO and store its ID
        if (m_RendererID == 0) {
            std::cerr << "Failed to generate VAO" << std::endl;
        }
    }
}
