#include <GL/glew.h> //To include glew, must include it before glfw3.h
#include "GlfwFunctions.h"
#include <iostream>
#include <GraphicsSystem.h>

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
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    GLFWFunctions::init(1600, 900, "Hello World");
    glfwMakeContextCurrent(GLFWFunctions::pWindow);

    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
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

    glfwSwapBuffers(GLFWFunctions::pWindow);

    glfwPollEvents();

    GLFWFunctions::showFPS(GLFWFunctions::pWindow);
    glfwSetWindowTitle(GLFWFunctions::pWindow, std::to_string(GLFWFunctions::fps).c_str());


    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
}

static void cleanup() {
    GLFWFunctions::glfwCleanup();
    glfwTerminate();
    graphicsSystem.Cleanup();
}