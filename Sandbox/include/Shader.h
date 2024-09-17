#pragma once

#include <GL/glew.h>
#include <string>

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    bool IsCompiled() const { return m_IsCompiled; }

private:
    GLuint m_ShaderID;
    bool m_IsCompiled;

    GLuint CompileShader(GLenum type, const std::string& source);
};
