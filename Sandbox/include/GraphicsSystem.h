#pragma once

#include <vector>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"

class GraphicsSystem {
public:
    GraphicsSystem();
    ~GraphicsSystem();

    void Initialize();
    void Update();
    void Render();
    void Cleanup();

private:
   
    VertexArray m_VAO;
    VertexBuffer* m_VBO; 
    Shader* m_Shader;
    std::vector<VertexArray*> m_Objects; 
};
