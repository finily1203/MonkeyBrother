#include "GraphicsSystem.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

GraphicsSystem::GraphicsSystem()
    : m_VBO(nullptr), m_Shader(nullptr) {
}

GraphicsSystem::~GraphicsSystem() {
    Cleanup();
}

void GraphicsSystem::Initialize() {
    // Initialize OpenGL settings
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the clear color to black

    // Example shader source code
    const std::string vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        void main() {
            gl_Position = vec4(position, 1.0);
        }
    )";

    const std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec4 u_Color;  // Color uniform
        void main() {
            FragColor = u_Color; // Use the color uniform
        }
    )";


    // Create and compile shaders
    m_Shader = new Shader(vertexShaderSource, fragmentShaderSource);
    if (!m_Shader->IsCompiled()) {
        std::cerr << "Shader compilation failed." << std::endl;
        return;
    }

    // Define vertices for a triangle
    float vertices[] = {
        0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    // Initialize VertexBuffer
    m_VBO = new VertexBuffer(vertices, sizeof(vertices));

  
    m_VAO.Bind();
    m_VBO->Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

  
    m_Objects.push_back(&m_VAO);
}

void GraphicsSystem::Update() {
    //empty for now
}

void GraphicsSystem::Render() {
   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind the shader
    m_Shader->Bind();

  
    for (auto& vao : m_Objects) {
        vao->Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3); 
    }
}

void GraphicsSystem::Cleanup() {
    if (m_VBO) {
        delete m_VBO;
        m_VBO = nullptr;
    }
    if (m_Shader) {
        delete m_Shader;
        m_Shader = nullptr;
    }
    m_Objects.clear();
}
