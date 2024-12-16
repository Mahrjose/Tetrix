#pragma once

#include <GL/glew.h>

/**
 * @class IndexBuffer
 * @brief Encapsulates an OpenGL Index Buffer Object (IBO) for storing indices used in indexed rendering.
 */
class IndexBuffer {
   private:
    unsigned int m_RendererID;  // OpenGL ID for the index buffer.
    unsigned int m_Count;       // Number of indices stored in the buffer.

   public:
    /**
     * @brief Constructs an IndexBuffer and uploads data to the GPU.
     *
     * Allocates GPU memory for the index buffer, uploads the given data,
     * and sets the usage pattern to `GL_STATIC_DRAW` for optimized rendering.
     *
     * @param data Pointer to the array of index data (e.g., GLuint indices).
     * @param count Number of indices in the buffer.
     */
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();

    /**
     * @brief Binds the IndexBuffer, making it the current element array buffer.
     */
    void Bind() const;

    /**
     * @brief Unbinds the current element array buffer.
     */
    void Unbind() const;

    /**
     * @brief Gets the number of indices stored in the buffer.
     *
     * @return The number of indices.
     */
    inline unsigned int getCount() const { return m_Count; };
};
