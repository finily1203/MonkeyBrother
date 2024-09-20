#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlfwFunctions.h"
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        std::cout << "Failed to open shader file: " << filepath << std::endl;
        return { "", "" };  // Return empty if the file can't be opened
    }

    enum class Shadertype {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    Shadertype type = Shadertype::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = Shadertype::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = Shadertype::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[512];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

// Declaration of the shader variable
static unsigned int shader;


namespace monkeybrother {
    __declspec(dllimport) void Print();
}

static void init();
static void update();
static void draw();
static void cleanup();

GraphicsSystem graphicsSystem;

int main() {
    monkeybrother::Print();
    init();
   
    graphicsSystem.Initialize();

    glfwSwapInterval(1); // Enable vertical synchronization

    while (!glfwWindowShouldClose(GLFWFunctions::pWindow)) {
        update();
        draw();
    }

   
    cleanup();

    return 0;
}

static void init() {
    GLFWFunctions::init(640, 480, "Hello World");

    // Vertex Buffer
    float positions[] = {
        -0.5f, -0.5f,
        0.5f,  -0.5f,
        0.5f,   0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[]{
        0, 1, 2,
        2, 3, 0
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    ShaderProgramSource source = ParseShader("Basic.shader");
    shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
}

    std::cout << "GLEW initialized successfully" << std::endl;

   
    int width, height;
    glfwGetFramebufferSize(GLFWFunctions::pWindow, &width, &height);
    glViewport(0, 0, width, height);

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
}

static void update() {
    // Update logic
}

static void draw() {
   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    graphicsSystem.Render();

    /*glBegin(GL_TRIANGLES);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, -0.5f);
    glEnd();*/

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    /* Swap front and back buffers */
    glfwSwapBuffers(GLFWFunctions::pWindow);

    glfwPollEvents();

    GLFWFunctions::showFPS(GLFWFunctions::pWindow);

    // Set the window title
    glfwSetWindowTitle(GLFWFunctions::pWindow, std::to_string(GLFWFunctions::fps).c_str());

   
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
}

static void cleanup() {
	GLFWFunctions::glfwCleanup();
    glDeleteProgram(shader);
}