#pragma once

#include <string>

class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();

    void CompileAndLink();
    void Use() const;
    void SetUniform1f(const std::string& name, float value) const;
    bool IsCompiled() const;
    void Bind() const;
    void Unbind() const; // Add this method declaration
    unsigned int GetRendererID() const;

private:
    unsigned int m_RendererID;
    bool m_IsCompiled;

    void CompileShader(unsigned int shaderID, const std::string& source);
    void LinkProgram(unsigned int vertexShader, unsigned int fragmentShader);
};
