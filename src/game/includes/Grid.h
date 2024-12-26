#pragma once

#include <iostream>
#include <memory>
#include <optional>
#include <vector>

#include "GLAlgorithms.h"
#include "Renderer.h"
#include "Shader.h"
#include "Tetromino.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Grid {
   private:
    int m_Width, m_Height, m_Cols, m_Rows;

    VertexArray m_VAO;
    std::unique_ptr<VertexBuffer> m_VBOPtr;
    Shader m_Shader;
    Renderer m_Renderer;

    std::vector<std::vector<int>> m_GameState;
    std::vector<float> m_GridVertices;

    void GenerateGridVertices();

   public:
    Grid();
    ~Grid();

    inline int GetCols() const { return m_Cols; };
    inline int GetRows() const { return m_Rows; };

    bool IsCellEmpty(int col, int row) const;
    int GetCellState(int col, int row) const;
    void SetCellState(int col, int row, int state);

    bool CanMoveTetromino(const Tetromino& tetromino, bool bottom, bool Left, bool Right) const;
    void PlaceTetromino(const Tetromino& tetromino);

    void Draw();
};
