#include "Grid.h"

#include <cmath>
#include <iostream>

/*
Game Board Info:
Window      : 683 x 738
Grid Height : 738 pixels
Grid Weight : 410 pixels
Grid Size   : 10 cols x 20 rows
Cell Size   : 41 x 37 pixels per cell
Start       : (x, y) -> (28,  0)
End         : (x, y) -> (438, 0)
*/

Grid::Grid()
    : m_Width(410), m_Height(738), m_Cols(10), m_Rows(20), m_Shader("../resources/shaders/Grid.glsl") {
    GenerateGridVertices();

    m_VBOPtr = std::make_unique<VertexBuffer>(m_GridVertices.data(), m_GridVertices.size() * sizeof(float));
    m_VBOPtr->Push<float>(2);
    m_VAO.AddBuffer(*m_VBOPtr);

    glm::mat4 proj = glm::ortho(0.0f, 683.0f, 0.0f, 738.0f, -1.0f, 1.0f);
    m_Shader.Bind();
    m_Shader.SetUniformMat4f("u_MVP", proj);
}

Grid::~Grid() {
}

void Grid::GenerateGridVertices() {
    int cellWidth = 41;
    int cellHeight = 37;
    int startX = 28;  // Start : (x, y) -> (28, 0)
    int startY = 0;

    for (unsigned int row = 0; row < m_Rows; row++) {
        for (unsigned int col = 0; col < m_Cols; col++) {
            // Top left of the current cell
            float x = startX + col * cellWidth;
            float y = startY + row * cellHeight;

            // Top-Left corner
            m_GridVertices.push_back(x);
            m_GridVertices.push_back(y);

            // Top-Rigth corner
            m_GridVertices.push_back(x + cellWidth);
            m_GridVertices.push_back(y);

            // Bottom-left corner
            m_GridVertices.push_back(x);
            m_GridVertices.push_back(y + cellHeight);

            // Bottom-right corner
            m_GridVertices.push_back(x + cellWidth);
            m_GridVertices.push_back(y + cellHeight);
        }
    }
}

void Grid::Draw() {
    m_Shader.Bind();
    m_VAO.Bind();
    m_Renderer.DrawPoints(0, m_VAO, m_Shader, 2.0f);
}

