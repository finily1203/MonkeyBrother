#include "GraphicsSystem.h"
#include "Shader.h"
#include <iostream>

GraphicsSystem::GraphicsSystem()
    : m_VAO(0), m_VBO(0), m_Texture(0) {
}

GraphicsSystem::~GraphicsSystem() {
    Cleanup();
}

GraphicsSystem::GraphicsSystem(const GraphicsSystem& other)
    : m_VAO(0), m_VBO(0), m_Texture(0) {
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
    : m_VAO(other.m_VAO), m_VBO(other.m_VBO), m_Texture(other.m_Texture), m_Shader(std::move(other.m_Shader)) {
    other.m_VAO = 0;
    other.m_VBO = 0;
    other.m_Texture = 0;
}

GraphicsSystem& GraphicsSystem::operator=(GraphicsSystem&& other) noexcept {
    if (this != &other) {
        Cleanup();

        m_VAO = other.m_VAO;
        m_VBO = other.m_VBO;
        m_Texture = other.m_Texture;
        m_Shader = std::move(other.m_Shader);

        other.m_VAO = 0;
        other.m_VBO = 0;
        other.m_Texture = 0;
    }
    return *this;
}

void GraphicsSystem::Initialize() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Shader sources
    const std::string vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec2 texCoord;

        out vec2 TexCoord;

        void main() {
            gl_Position = vec4(position, 1.0);
            TexCoord = texCoord;
        }
    )";

    const std::string fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        in vec2 TexCoord;

        uniform sampler2D u_Texture;

        void main() {
            FragColor = texture(u_Texture, TexCoord);
        }
    )";

    m_Shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    if (!m_Shader->IsCompiled()) {
        std::cerr << "Shader compilation failed." << std::endl;
        return;
    }

    
    float vertices[] = {
        // Positions         // TexCoords
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   
    };

    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

   
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    glGenTextures(1, &m_Texture);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    
    unsigned char textureData[] = {
        // Row 1
        255, 255, 255,   // White
        0, 0, 0,         // Black
        255, 255, 255,   // White
        0, 0, 0,         // Black

        // Row 2
        0, 0, 0,         // Black
        255, 255, 255,   // White
        0, 0, 0,         // Black
        255, 255, 255    // White
    };

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 4, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindVertexArray(0);
}

void GraphicsSystem::Update() {
    //empty for now
}

void GraphicsSystem::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader->Bind();
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 

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
    if (m_Texture) {
        glDeleteTextures(1, &m_Texture);
        m_Texture = 0;
    }
    if (m_Shader) {
        m_Shader.reset();
    }
}
