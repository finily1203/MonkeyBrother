#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlfwFunctions.h"
#include <iostream>
#include "Renderer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "GraphicsSystem.h"

namespace monkeybrother {
    __declspec(dllimport) void Print();
}

static void init();
static void update();
static void draw();
static void cleanup();

GraphicsSystem graphics;

int main() {
    monkeybrother::Print();
    init();
    graphics.Initialize();

    glfwSwapInterval(1); // Enable vertical synchronization

    while (!glfwWindowShouldClose(GLFWFunctions::pWindow)) {
        update();
        draw();
    }

    graphics.Cleanup();
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

    // Set the viewport to match the window size
    int width, height;
    glfwGetFramebufferSize(GLFWFunctions::pWindow, &width, &height);
    glViewport(0, 0, width, height);

    // Set the clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
}

static void update() {
    // Update logic
}

static void draw() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    graphics.Render();

    // Swap front and back buffers
    glfwSwapBuffers(GLFWFunctions::pWindow);

    // Poll for and process events
    glfwPollEvents();

    // Update window title with FPS
    GLFWFunctions::showFPS(GLFWFunctions::pWindow);
    glfwSetWindowTitle(GLFWFunctions::pWindow, std::to_string(GLFWFunctions::fps).c_str());

    // Check for OpenGL errors
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << error << std::endl;
    }
}

static void cleanup() {
    GLFWFunctions::glfwCleanup();
    glfwTerminate(); // Ensure GLFW is properly terminated
    graphics.Cleanup();
}
