#pragma once

#include <GL/glew.h>
#include <memory>  

class Shader;

class GraphicsSystem {
public:
   
    GraphicsSystem();

   
    ~GraphicsSystem();

   
    GraphicsSystem(const GraphicsSystem& other);             
    GraphicsSystem& operator=(const GraphicsSystem& other);  
    GraphicsSystem(GraphicsSystem&& other) noexcept;         
    GraphicsSystem& operator=(GraphicsSystem&& other) noexcept;  

   
    void Initialize();
    void Update();
    void Render();
    void Cleanup();

private:
    GLuint m_VAO;  
    GLuint m_VBO; 
    std::unique_ptr<Shader> m_Shader;  

   
    void ReleaseResources();
};
