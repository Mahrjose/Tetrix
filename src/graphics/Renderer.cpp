#pragma once

#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

/**
 * @class Renderer
 * @brief Handles rendering operations for OpenGL.
 *
 * Provides methods to clear the screen and draw various primitives
 * (e.g., triangles and points) using the provided vertex array, index buffer,
 * and shaders.
 */
class Renderer {
   public:
    /**
     * @brief Clears the color buffer to reset the screen.
     *
     * Calls `glClear` with `GL_COLOR_BUFFER_BIT` to reset the color buffer.
     */
    void ClearScreen() const;

    /**
     * @brief Draws triangles using the specified VertexArray, IndexBuffer, and Shader.
     *
     * - Binds the provided shader, vertex array, and index buffer.
     * - Calls `glDrawElements` to render the triangles based on the index buffer.
     *
     * @param va The VertexArray containing the vertex data.
     * @param ib The IndexBuffer containing indices for rendering triangles.
     * @param shader The Shader program to use for rendering.
     */
    void DrawTringles(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

    /**
     * @brief Draws points using the specified VertexArray and Shader.
     *
     * - Binds the provided shader and vertex array.
     * - Sets the point size and calls `glDrawArrays` with `GL_POINT` mode to render points.
     *
     * @param start The starting index of the points in the VertexArray.
     * @param va The VertexArray containing the vertex data.
     * @param shader The Shader program to use for rendering.
     * @param pointSize The size of the points to render. Default is 1.0.
     */
    void DrawPoints(const unsigned int& start, const VertexArray& va, const Shader& shader, const float& pointSize = 1.0f) const;
};
