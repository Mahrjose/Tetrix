#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr size)
    : m_Size(size), m_Stride(0) {
    glGenBuffers(1, &m_RendererID);                             // Generate a buffer ID.
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);                // Bind the buffer as an ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);  // Upload data to GPU.
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
