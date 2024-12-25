#pragma once

#include <GL/glew.h>

#include <vector>

struct VertexBufferElement {
    unsigned int type;        // The data type (e.g., GL_FLOAT, GL_UNSIGNED_INT).
    unsigned int count;       // Number of elements of the given type.
    unsigned int normalized;  // Whether the data is normalized (e.g., GL_TRUE or GL_FALSE).

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT:
                return 4;
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
        }
        return 0;  // Error type
    }
};

class VertexBuffer {
   private:
    GLuint m_RendererID;                          // OpenGL ID for the vertex buffer.
    GLsizeiptr m_Size;                            // Total size of the vertex data in bytes.
    unsigned int m_Stride;                        // Total stride of the vertex layout in bytes.
    std::vector<VertexBufferElement> m_Elements;  // Layout elements of the buffer.

   public:
    /**
     * @param data A pointer to the vertex data.
     * @param size The size of the vertex data in bytes.
     */
    VertexBuffer(const void* data, GLsizeiptr size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    /**
     * @brief Updates the buffer with new data.
     * 
     * If the new size is smaller or equal to the existing size, updates only the modified portion.
     * Otherwise, reallocates the buffer with the new size.
     * 
     * @param data Pointer to the new vertex data.
     * @param size Size of the new vertex data in bytes.
     */
    void Update(const void* data, GLsizeiptr size);

    /**
     * @return The size of the vertex buffer in bytes.
     */
    inline GLsizeiptr GetSize() const { return m_Size; };

    /**
     * @return The stride (in bytes) of the vertex layout.
     */
    inline unsigned int GetStride() const { return m_Stride; };

    /**
     * @return A reference to the vector of VertexBufferElements.
     */
    inline const std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };

    /**
     * @tparam T The data type of the element (e.g., float, unsigned int).
     * @param count The number of elements of the given type.
     */
    template <typename T>
    void Push(unsigned int count) {
        static_assert(sizeof(T) == 0, "Unsupported type for VertexBuffer::Push");
    }
};

template <>
inline void VertexBuffer::Push<float>(unsigned int count) {
    m_Elements.push_back({GL_FLOAT, static_cast<unsigned int>(count), GL_FALSE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template <>
inline void VertexBuffer::Push<unsigned int>(unsigned int count) {
    m_Elements.push_back({GL_UNSIGNED_INT, static_cast<unsigned int>(count), GL_FALSE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template <>
inline void VertexBuffer::Push<unsigned char>(unsigned int count) {
    m_Elements.push_back({GL_UNSIGNED_BYTE, static_cast<unsigned int>(count), GL_TRUE});
    m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}
