#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "GLAlgorithms.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class ShapeType { I,
                       L,
                       O };

class Tetromino {
   private:
    VertexArray m_VAO;
    std::unique_ptr<VertexBuffer> m_VBOPtr;
    Shader m_Shader;
    Renderer m_Renderer;

    std::vector<float> m_BoxVertices;
    std::vector<std::pair<int, int>> m_BlockPositions;

   public:
    Tetromino();
    // ~Tetromino();

    void CreateBox(int row, int col);
    void UpdateBuffer();

    void MoveRight();
    void MoveLeft();
    void MoveDown();

    void SetShape(int baseRow, int baseCol, ShapeType shape);
    void Draw();

    const std::vector<std::pair<int, int>>& GetBlockPositions() const { return m_BlockPositions; }
};
