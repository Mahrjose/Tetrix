#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, GLsizeiptr size)
    : m_Size(size), m_Stride(0) {
    glGenBuffers(1, &m_RendererID);                             // Generate a buffer ID.
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);                // Bind the buffer as an ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);  // Upload data to GPU.
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Update(const void* data, GLsizeiptr size) {
    Bind();  // Bind the buffer to ensure it is active.

    if (size <= m_Size) {
        // Update the buffer data without reallocating.
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    } else {
        // Reallocate and update if the new size is larger.
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
        m_Size = size;  // Update the stored buffer size.
    }
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
