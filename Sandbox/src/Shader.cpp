#include "Shader.h"
#include <GL/glew.h>
#include <iostream>

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
    : m_IsCompiled(false), m_RendererID(0) {
    CompileAndLink();
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}


void Shader::CompileAndLink() {
    // Compilation and linking logic here
    // Once successful:
    m_IsCompiled = true;
}

void Shader::Bind() const {
    glUseProgram(m_RendererID); // Bind the shader program
}

void Shader::Unbind() const {
    glUseProgram(0); // Unbind the shader program by setting the active program to 0
}

void Shader::SetUniform1f(const std::string& name, float value) const {
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
    }
    glUniform1f(location, value);
}

bool Shader::IsCompiled() const {
    return m_IsCompiled;
}
