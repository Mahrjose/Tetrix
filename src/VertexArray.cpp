#include "VertexArray.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer& vb) {
    Bind();     // Bind the VAO.
    vb.Bind();  // Bind the associated VertexBuffer.

    const auto& elements = vb.GetElements();
    unsigned int offset = 0;

    for (unsigned int attributeIndex = 0; attributeIndex < elements.size(); attributeIndex++) {
        const auto element = elements[attributeIndex];

        glEnableVertexAttribArray(attributeIndex);
        glVertexAttribPointer(
            attributeIndex,      // Index of the attribute.
            element.count,       // Number of components (e.g., 3 for vec3).
            element.type,        // Data type (e.g., GL_FLOAT).
            element.normalized,  // Normalize data or not (e.g., GL_TRUE).
            vb.GetStride(),      // Stride: Distance between consecutive vertices.
            (const void*)offset  // Offset: Start position in the buffer for this attribute.
        );

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }

    m_NumVertices = vb.GetSize() / vb.GetStride();
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}
