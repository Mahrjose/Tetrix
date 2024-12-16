#pragma once

#include "VertexBuffer.h"

class VertexArray {
   private:
    unsigned int m_RendererID;   // OpenGL ID for the Vertex Array Object.
    unsigned int m_NumVertices;  // Total number of vertices in the associated VertexBuffer.

   public:
    VertexArray();
    ~VertexArray();

    /**
     * @param vb The VertexBuffer containing vertex data and its layout.
     */
    void AddBuffer(const VertexBuffer& vb);

    void Bind() const;
    void Unbind() const;

    /// Retrieves the total number of vertices in the VertexBuffer.
    /**
     * @return The number of vertices stored in the buffer.
     */
    inline unsigned int GetNumVertices() const { return m_NumVertices; };
};
