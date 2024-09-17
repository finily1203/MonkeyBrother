#include "GraphicsSystem.h"
#include "Shader.h"
#include <iostream>

GraphicsSystem::GraphicsSystem()
    : m_VAO(0), m_VBO(0) {
}

GraphicsSystem::~GraphicsSystem() {
    Cleanup(); 
}


GraphicsSystem::GraphicsSystem(const GraphicsSystem& other)
    : m_VAO(0), m_VBO(0) {
    if (other.m_Shader) {
        m_Shader = std::make_unique<Shader>(*other.m_Shader);
    }
}


GraphicsSystem& GraphicsSystem::operator=(const GraphicsSystem& other) {
    if (this != &other) {
        Cleanup(); 

        if (other.m_Shader) {
            m_Shader = std::make_unique<Shader>(*other.m_Shader);
        }
    }
    return *this;
}


GraphicsSystem::GraphicsSystem(GraphicsSystem&& other) noexcept
    : m_VAO(other.m_VAO), m_VBO(other.m_VBO), m_Shader(std::move(other.m_Shader)) {
    other.m_VAO = 0;
    other.m_VBO = 0;
}


GraphicsSystem& GraphicsSystem::operator=(GraphicsSystem&& other) noexcept {
    if (this != &other) {
        Cleanup(); 

        m_VAO = other.m_VAO;
        m_VBO = other.m_VBO;
        m_Shader = std::move(other.m_Shader);

        other.m_VAO = 0;
        other.m_VBO = 0;
    }
    return *this;
}

void GraphicsSystem::Initialize() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        void main() {
            FragColor = vec4(1.0, .0, 1.0, 1.0); // Red color
        }
    )";

    m_Shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    if (!m_Shader->IsCompiled()) {
        std::cerr << "Shader compilation failed." << std::endl;
        return;
    }

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,  
         0.5f, -0.5f, 0.0f, 
         0.0f,  0.5f, 0.0f   
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // unbind VAO
}

void GraphicsSystem::Update() {
    // empty for now
}

void GraphicsSystem::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->Bind();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); // do triangle first
    glBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}

void GraphicsSystem::Cleanup() {
    ReleaseResources();
}

void GraphicsSystem::ReleaseResources() {
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_Shader) {
        m_Shader.reset(); 
    }
}
