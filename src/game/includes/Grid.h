#pragma once

#include <memory>
#include <optional>

#include "GLAlgorithms.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Grid {
   private:
    unsigned int m_Width, m_Height, m_Cols, m_Rows;

    VertexArray m_VAO;
    std::unique_ptr<VertexBuffer> m_VBOPtr;
    Shader m_Shader;
    Renderer m_Renderer;

    std::vector<std::vector<unsigned int>> m_GameState;
    std::vector<float> m_drawVertices;
    std::vector<float> m_GridVertices;

    void GenerateGridVertices();

   public:
    Grid();
    ~Grid();

    // void SetCellState(unsigned int x, unsigned int y, int state);
    void Draw();
};
