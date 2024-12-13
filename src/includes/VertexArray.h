#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray {
   private:
    unsigned int m_RendererID;  // ID for vertex Array
    unsigned int m_NumVertices; // Number of vertices (change comment)
    
   public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetNumVertices() const { return m_NumVertices; };
};
