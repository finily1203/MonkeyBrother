#pragma once

#include "VertexArray.h"
#include "Shader.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray& vao, const Shader& shader) const;


};
