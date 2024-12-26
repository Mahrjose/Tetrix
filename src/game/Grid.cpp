#include "Grid.h"

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
    : m_Width(410), m_Height(738), m_Cols(10), m_Rows(24), m_Shader("../resources/shaders/Grid.glsl") {
    GenerateGridVertices();

    m_VBOPtr = std::make_unique<VertexBuffer>(m_GridVertices.data(), m_GridVertices.size() * sizeof(float));
    m_VBOPtr->Push<float>(2);
    m_VAO.AddBuffer(*m_VBOPtr);

    glm::mat4 proj = glm::ortho(0.0f, 683.0f, 0.0f, 738.0f, -1.0f, 1.0f);
    m_Shader.Bind();
    m_Shader.SetUniformMat4f("u_MVP", proj);

    m_GameState.resize(m_Rows, std::vector<int>(m_Cols, 0));
}

Grid::~Grid() {
}

void Grid::GenerateGridVertices() {
    int cellWidth = 41;
    int cellHeight = 36;
    int startX = 28;  // Start : (x, y) -> (28, 0)
    int startY = 3;

    for (int row = 0; row < m_Rows; row++) {
        for (int col = 0; col <= m_Cols; col++) {
            float x = startX + col * cellWidth;
            float y = startY + row * cellHeight;

            // Bottom-to-Top (left edge)
            GLAlgorithms::DrawLine(x, y, x, y + cellHeight, m_GridVertices);

            // Left-to-Right (bottom edge)
            GLAlgorithms::DrawLine(x, y, x + cellWidth, y, m_GridVertices);

            // Right-to-Top (right edge)
            GLAlgorithms::DrawLine(x + cellWidth, y, x + cellWidth, y + cellHeight, m_GridVertices);

            // Top-to-Left (top edge)
            GLAlgorithms::DrawLine(x, y + cellHeight, x + cellWidth, y + cellHeight, m_GridVertices);
        }
    }
}

bool Grid::IsCellEmpty(int col, int row) const {
    return m_GameState[row][col] == 0;
}

int Grid::GetCellState(int col, int row) const {
    return m_GameState[row][col];
}

void Grid::SetCellState(int row, int col, int state) {
    m_GameState[row][col] = state;
}

bool Grid::CanMoveTetromino(const Tetromino& tetromino, bool bottom, bool left, bool right) const {
    // Determine the direction of movement
    int deltaRow = 0, deltaCol = 0;
    if (bottom) deltaRow = -1;  // Moving down decreases the row
    if (left) deltaCol = -1;    // Moving left decreases the column
    if (right) deltaCol = 1;    // Moving right increases the column

    for (const auto& [row, col] : tetromino.GetBlockPositions()) {
        int newRow = row + deltaRow;
        int newCol = col + deltaCol;

        // Check grid boundaries
        if (newRow < 0 || newRow >= m_Rows) return false;  // Vertical boundary check
        if (newCol < 0 || newCol >= m_Cols) return false;  // Horizontal boundary check

        if (!IsCellEmpty(newCol, newRow)) return false;
    }
    return true;
}

void Grid::PlaceTetromino(const Tetromino& tetromino) {
    for (const auto& [row, col] : tetromino.GetBlockPositions()) {
        SetCellState(row, col, 1);  // Mark as occupied
    }

    // Add the Tetromino's vertex data to the grid's draw buffer
    m_GridVertices.insert(
        m_GridVertices.end(),
        tetromino.GetVertexData().begin(),
        tetromino.GetVertexData().end()
    );

    // Update the grid's OpenGL buffer with new vertices
    m_VBOPtr->Update(m_GridVertices.data(), m_GridVertices.size() * sizeof(float));
    m_VAO.AddBuffer(*m_VBOPtr);
}

void Grid::Draw() {
    m_Shader.Bind();
    m_VAO.Bind();
    m_Renderer.DrawPoints(0, m_VAO, m_Shader, 2.0f);
}
