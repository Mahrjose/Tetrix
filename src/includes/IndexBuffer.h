#pragma once

#include <GL/glew.h>

class IndexBuffer {
   private:
    unsigned int m_RendererID;  // ID for the Buffer
    unsigned int m_Count;       // The number of indices in the index buffer

   public:
    IndexBuffer(const void* data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int getCount() const { return m_Count; };
};
