#pragma once

#include <GL/glew.h>

class VertexArray {
public:
    VertexArray();      // Constructor
    ~VertexArray();     // Destructor

    void Bind() const;    // Bind the VAO
    void Unbind() const;  // Unbind the VAO
    void Initialize();    // Explicitly initialize the VAO

private:
    unsigned int m_RendererID; // VAO ID
};
