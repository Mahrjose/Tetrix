#include "Tetromino.h"

#include <algorithm>
#include <iostream>

Tetromino::Tetromino()
    : m_Shader("../resources/shaders/Tetromino.glsl") {
    m_VBOPtr = std::make_unique<VertexBuffer>(m_BoxVertices.data(), m_BoxVertices.size() * sizeof(float));
    m_VBOPtr->Push<float>(2);
    m_VAO.AddBuffer(*m_VBOPtr);

    glm::mat4 proj = glm::ortho(0.0f, 683.0f, 0.0f, 738.0f, -1.0f, 1.0f);
    m_Shader.Bind();
    m_Shader.SetUniformMat4f("u_MVP", proj);
}

void Tetromino::CreateBox(int row, int col) {
    int cellWidth = 41;
    int cellHeight = 36;
    int startX = 28;
    int startY = 3;

    // Calculate the position of the box
    float x = startX + col * cellWidth;
    float y = startY + row * cellHeight;

    // Fill the box by drawing horizontal lines from bottom to top
    for (int i = 0; i < cellHeight; i++) {
        float currentY = y + i;
        GLAlgorithms::DrawLine(x, currentY, x + cellWidth, currentY, m_BoxVertices);
    }

    UpdateBuffer();
}

void Tetromino::SetShape(int baseRow, int baseCol, ShapeType shape) {
    m_BlockPositions.clear();
    m_BoxVertices.clear();

    switch (shape) {
        case ShapeType::I:
            for (int i = 0; i < 4; i++) m_BlockPositions.emplace_back(baseRow + i, baseCol);
            break;

        case ShapeType::L:
            for (int i = 0; i < 3; i++) m_BlockPositions.emplace_back(baseRow + i, baseCol);
            m_BlockPositions.emplace_back(baseRow + 2, baseCol + 1);
            break;

        case ShapeType::O:
            m_BlockPositions.emplace_back(baseRow, baseCol);
            m_BlockPositions.emplace_back(baseRow, baseCol + 1);
            m_BlockPositions.emplace_back(baseRow + 1, baseCol);
            m_BlockPositions.emplace_back(baseRow + 1, baseCol + 1);
            break;
    }

    for (const auto& [row, col] : m_BlockPositions) {  // Unpack into row and col
        CreateBox(row, col);
    }
    UpdateBuffer();
}

void Tetromino::MoveDown() {
    for (auto& [row, col] : m_BlockPositions) {
        row -= 1;  // Move row down by 1
    }

    m_BoxVertices.clear();
    for (const auto& [row, col] : m_BlockPositions) {
        CreateBox(row, col);
    }

    UpdateBuffer();
}

void Tetromino::MoveLeft() {
    for (auto& [row, col] : m_BlockPositions) {
        col -= 1;  // Move column left by 1
    }

    m_BoxVertices.clear();
    for (const auto& [row, col] : m_BlockPositions) {
        CreateBox(row, col);
    }

    UpdateBuffer();
}

void Tetromino::MoveRight() {
    for (auto& [row, col] : m_BlockPositions) {
        col += 1;  // Move column right by 1
    }

    m_BoxVertices.clear();
    for (const auto& [row, col] : m_BlockPositions) {
        CreateBox(row, col);
    }

    UpdateBuffer();
}

void Tetromino::UpdateBuffer() {
    // Update the VBO with the new vertex data
    m_VBOPtr->Update(m_BoxVertices.data(), m_BoxVertices.size() * sizeof(float));
    m_VAO.AddBuffer(*m_VBOPtr);
}

void Tetromino::Draw() {
    m_Shader.Bind();
    m_VAO.Bind();
    m_Renderer.DrawPoints(0, m_VAO, m_Shader, 2.0f);
}