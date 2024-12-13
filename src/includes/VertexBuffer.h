#pragma once

#include <GL/glew.h>

#include "VertexBufferLayout.h"

class VertexBuffer {
   private:
    unsigned int m_RendererID;  // ID for the vertex buffer
    unsigned int m_Size;        // Size of the vertex data

   public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetSize() const { return m_Size; };
};
