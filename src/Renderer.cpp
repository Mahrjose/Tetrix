#include "includes/Renderer.h"

#include <iostream>

void Renderer::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawT(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawP(const unsigned int& start, const VertexArray& va, const Shader& shader, const float& pointSize) const {
    shader.Bind();
    va.Bind();
    glPointSize(pointSize);
    glDrawArrays(GL_POINT, start, va.GetNumVertices());
}