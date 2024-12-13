#pragma once

#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

class Renderer {
   public:
    void Clear() const;
    void DrawT(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void DrawP(const unsigned int& start, const VertexArray& va, const Shader& shader, const float& pointSize = 1.0f) const;
};