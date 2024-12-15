#pragma once

#include <GL/glew.h>

#include "VertexBufferLayout.h"

class VertexBuffer {
   private:
    GLuint m_RendererID;  // ID for the vertex buffer
    GLsizeiptr m_Size;    // Size of the vertex data

   public:
    VertexBuffer(const void* data, GLsizeiptr size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    inline GLsizeiptr GetSize() const { return m_Size; };
};
