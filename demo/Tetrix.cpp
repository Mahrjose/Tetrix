#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

// -------------------------------- CONSTANTS & STRUCTURES -------------------------------- //

// Grid and Window Configuration
const int WINDOW_WIDTH = 683;
const int WINDOW_HEIGHT = 738;

const int GRID_ROWS = 20;
const int GRID_COLS = 10;
const int CELL_WIDTH = 41;
const int CELL_HEIGHT = 36;
const int START_X = 28;
const int START_Y = 3;

const int PREVIEW_X = START_X + (GRID_COLS * CELL_WIDTH) + 50;
const int PREVIEW_Y = START_Y + (GRID_ROWS - 4) * CELL_HEIGHT;

// Game Mechanics
const float INITIAL_FALL_DELAY = 0.5f;
const float FAST_FALL_DELAY = 0.05f;
const double MOVE_DELAY = 0.1;

// Scoring System
const int SCORE_SINGLE = 1;
const int SCORE_DOUBLE = 3;
const int SCORE_TRIPLE = 5;
const int SCORE_TETRIS = 8;

// Color Structure
struct Color {
    float r, g, b;
    Color() : r(0.0f), g(0.0f), b(0.0f) {}
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

const std::vector<Color> COLORS = {
    Color(0.0f, 1.0f, 0.0f),  // Green
    Color(0.0f, 0.0f, 1.0f),  // Blue
    Color(1.0f, 0.0f, 0.0f),  // Red
    Color(1.0f, 1.0f, 0.0f),  // Yellow
    Color(1.0f, 0.0f, 1.0f),  // Magenta
    Color(0.0f, 1.0f, 1.0f)   // Cyan
};

// Grid Cell Structure
struct Cell {
    bool occupied = false;
    Color color = Color(0.0f, 0.0f, 0.0f);
};

// Input State Structure
struct InputState {
    bool leftPressed = false;
    bool rightPressed = false;
    bool rotatePressed = false;
    bool pausePressed = false;
    double lastMoveTime = 0.0;
    const double moveDelay = MOVE_DELAY;
} inputState;

// Tetromino shapes
const std::vector<std::vector<std::vector<std::pair<int, int>>>> TETROMINOS = {
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}},  // I
    {{{0, 0}, {1, 0}, {2, 0}, {2, 1}}},  // L
    {{{0, 1}, {1, 1}, {2, 1}, {2, 0}}},  // J
    {{{0, 0}, {0, 1}, {1, 0}, {1, 1}}},  // O
    {{{0, 1}, {1, 0}, {1, 1}, {2, 0}}},  // S
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}},  // Z
    {{{0, 1}, {1, 0}, {1, 1}, {1, 2}}}   // T
};

// Tetromino Structure
struct Tetromino {
    std::vector<std::pair<int, int>> blocks;
    int type = 0;
    int rotation = 0;
    Color color;
};

// Global Game State
std::vector<std::vector<Cell>> grid(GRID_ROWS, std::vector<Cell>(GRID_COLS));
int score = 0;
bool gameOver = false;
bool gamePaused = false;

// -------------------------------- DRAWING FUNCTIONS -------------------------------- //

// Line Drawling Algorithm
void MPL(int x1, int y1, int x2, int y2, const Color& color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_POINTS);

    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        glVertex2i(x1, y1);

        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    glEnd();
}

// Fill the tetromino boxes with points to show colors
void FillBoxPoints(int x1, int y1, int width, int height, const Color& color) {
    glColor3f(color.r, color.g, color.b);
    glBegin(GL_POINTS);
    // Fill box with points
    for (int y = y1; y < y1 + height; ++y) {
        for (int x = x1; x < x1 + width; ++x) {
            glVertex2i(x, y);
        }
    }
    glEnd();
}

// Game Board (Grid system with row, col)
void DrawGrid() {
    Color gridColor(0.5f, 0.5f, 0.5f);

    // Draw vertical lines
    for (int col = 0; col <= GRID_COLS; ++col) {
        int x = START_X + col * CELL_WIDTH;
        MPL(x, START_Y, x, START_Y + GRID_ROWS * CELL_HEIGHT, gridColor);
    }

    // Draw horizontal lines
    for (int row = 0; row <= GRID_ROWS; ++row) {
        int y = START_Y + row * CELL_HEIGHT;
        MPL(START_X, y, START_X + GRID_COLS * CELL_WIDTH, y, gridColor);
    }
}

// Draw the Tetromino boxes with same size as a single grid cell
void DrawBox(int row, int col, const Color& color) {
    int x = START_X + col * CELL_WIDTH;
    int y = START_Y + (GRID_ROWS - row - 1) * CELL_HEIGHT;
    FillBoxPoints(x + 1, y + 1, CELL_WIDTH - 1, CELL_HEIGHT - 1, color);
}

void DrawPreviewBox(int row, int col, const Color& color) {
    int x = PREVIEW_X + col * CELL_WIDTH;
    int y = PREVIEW_Y + row * CELL_HEIGHT;
    FillBoxPoints(x + 1, y + 1, CELL_WIDTH - 1, CELL_HEIGHT - 1, color);
}

void DrawPreviewArea() {
    Color borderColor(0.5f, 0.5f, 0.5f);
    int width = 4 * CELL_WIDTH;
    int height = 4 * CELL_HEIGHT;

    // Draw preview area border
    MPL(PREVIEW_X, PREVIEW_Y, PREVIEW_X + width, PREVIEW_Y, borderColor);
    MPL(PREVIEW_X, PREVIEW_Y + height, PREVIEW_X + width, PREVIEW_Y + height, borderColor);
    MPL(PREVIEW_X, PREVIEW_Y, PREVIEW_X, PREVIEW_Y + height, borderColor);
    MPL(PREVIEW_X + width, PREVIEW_Y, PREVIEW_X + width, PREVIEW_Y + height, borderColor);
}

void DrawGameOver() {
    Color gameOverColor(1.0f, 0.0f, 0.0f);  // Red

    // Draw centered game over text box
    int centerX = WINDOW_WIDTH / 2;
    int centerY = WINDOW_HEIGHT / 2;
    int boxWidth = 300;
    int boxHeight = 200;

    // Draw box outline
    MPL(centerX - boxWidth / 2, centerY - boxHeight / 2,
        centerX + boxWidth / 2, centerY - boxHeight / 2, gameOverColor);
    MPL(centerX + boxWidth / 2, centerY - boxHeight / 2,
        centerX + boxWidth / 2, centerY + boxHeight / 2, gameOverColor);
    MPL(centerX + boxWidth / 2, centerY + boxHeight / 2,
        centerX - boxWidth / 2, centerY + boxHeight / 2, gameOverColor);
    MPL(centerX - boxWidth / 2, centerY + boxHeight / 2,
        centerX - boxWidth / 2, centerY - boxHeight / 2, gameOverColor);

    // Draw X pattern inside box
    MPL(centerX - boxWidth / 3, centerY - boxHeight / 3,
        centerX + boxWidth / 3, centerY + boxHeight / 3, gameOverColor);
    MPL(centerX + boxWidth / 3, centerY - boxHeight / 3,
        centerX - boxWidth / 3, centerY + boxHeight / 3, gameOverColor);
}

// -------------------------------- GAME LOGIC -------------------------------- //

Tetromino GenerateTetromino() {
    Tetromino tetromino;
    // tetromino.type = 3;
    tetromino.type = std::rand() % TETROMINOS.size();
    tetromino.color = COLORS[std::rand() % COLORS.size()];
    int col = std::rand() % (GRID_COLS - 3);

    tetromino.blocks = TETROMINOS[tetromino.type][0];
    for (auto& block : tetromino.blocks) {
        block.second += col;
    }

    if (std::rand() % 10 == 0) {  // 10% chance to generate dead tetromino
        Tetromino deadTetromino;
        deadTetromino.type = -1;                        // Special identifier
        deadTetromino.color = Color(0.5f, 0.5f, 0.5f);  // Gray
        int col = std::rand() % (GRID_COLS - 3);
        deadTetromino.blocks = TETROMINOS[0][0];  // Any shape
        for (auto& block : deadTetromino.blocks) {
            block.second += col;
        }
        return deadTetromino;
    }

    if (std::rand() % 10 == 0) {
        // if (std::rand() % 100 < 80) {  // 6.67% chance for bomb
        Tetromino bomb;
        bomb.type = -2;                        // Special identifier
        bomb.color = Color(0.5f, 0.5f, 0.0f);  // Black for bomb
        bomb.blocks = {{{0, GRID_COLS / 2}}};  // Single block
        return bomb;
    }

    return tetromino;
}

bool IsValidPosition(const Tetromino& tetromino) {
    for (const auto& block : tetromino.blocks) {
        int row = block.first;
        int col = block.second;

        if (row < 0 || row >= GRID_ROWS || col < 0 || col >= GRID_COLS ||
            (row >= 0 && grid[row][col].occupied)) {
            return false;
        }
    }
    return true;
}

void RotateTetromino(Tetromino& tetromino) {
    if (tetromino.type == 3) return;  // Don't rotate O piece

    std::vector<std::pair<int, int>> original = tetromino.blocks;
    int centerRow = tetromino.blocks[1].first;
    int centerCol = tetromino.blocks[1].second;

    // Rotate around center point
    for (auto& block : tetromino.blocks) {
        int row = block.first - centerRow;
        int col = block.second - centerCol;
        block.first = centerRow - col;
        block.second = centerCol + row;
    }

    // Revert if invalid position
    if (!IsValidPosition(tetromino)) {
        tetromino.blocks = original;
    }
}

void MoveDown(Tetromino& tetromino) {
    for (auto& block : tetromino.blocks) {
        block.first += 1;
    }
}

void MoveLeft(Tetromino& tetromino) {
    for (auto& block : tetromino.blocks) {
        block.second -= 1;
    }
}

void MoveRight(Tetromino& tetromino) {
    for (auto& block : tetromino.blocks) {
        block.second += 1;
    }
}

bool CanMoveDown(const Tetromino& tetromino) {
    Tetromino test = tetromino;
    MoveDown(test);
    return IsValidPosition(test);
}

bool CanMoveLeft(const Tetromino& tetromino) {
    Tetromino test = tetromino;
    MoveLeft(test);
    return IsValidPosition(test);
}

bool CanMoveRight(const Tetromino& tetromino) {
    Tetromino test = tetromino;
    MoveRight(test);
    return IsValidPosition(test);
}

void LockTetromino(const Tetromino& tetromino) {
    for (const auto& block : tetromino.blocks) {
        if (block.first >= 0) {
            grid[block.first][block.second].occupied = true;
            grid[block.first][block.second].color = tetromino.color;
        }
        if (tetromino.type == -2) {             // Bomb effect
            for (int dr = -2; dr <= 2; ++dr) {  // Adjust radius
                for (int dc = -2; dc <= 2; ++dc) {
                    int row = block.first + dr;
                    int col = block.second + dc;
                    if (row >= 0 && row < GRID_ROWS && col >= 0 && col < GRID_COLS) {
                        grid[row][col].occupied = false;
                    }
                }
            }
            return;
        }
    }
}

bool CanClearLine(int row) {
    for (int col = 0; col < GRID_COLS; ++col) {
        if (grid[row][col].occupied && grid[row][col].color.r == 0.5f) {  // Check for gray blocks
            return false;
        }
    }
    return true;
}

void ClearLines() {
    int linesCleared = 0;

    // Check each row from bottom to top
    for (int row = GRID_ROWS - 1; row >= 0; --row) {
        bool lineFull = true;
        if (!CanClearLine(row)) continue;
        for (int col = 0; col < GRID_COLS; ++col) {
            if (!grid[row][col].occupied) {
                lineFull = false;
                break;
            }
        }

        if (lineFull) {
            linesCleared++;
            // Move all rows above down
            for (int r = row; r > 0; --r) {
                grid[r] = grid[r - 1];
            }
            // Clear top row
            for (int col = 0; col < GRID_COLS; ++col) {
                grid[0][col].occupied = false;
            }
            row++;  // Check same row again
        }
    }

    // Update score
    switch (linesCleared) {
        case 1:
            score += SCORE_SINGLE;
            break;
        case 2:
            score += SCORE_DOUBLE;
            break;
        case 3:
            score += SCORE_TRIPLE;
            break;
        case 4:
            score += SCORE_TETRIS;
            break;
    }

    if (linesCleared > 0) {
        std::cout << "Score: " << score << std::endl;
    }
}

void ResetGame() {
    gameOver = false;
    gamePaused = false;
    score = 0;
    // Clear grid
    grid = std::vector<std::vector<Cell>>(GRID_ROWS, std::vector<Cell>(GRID_COLS));
    std::cout << "Game Restarted!\nScore: 0" << std::endl;
}

void CheckGameOver(const Tetromino& tetromino) {
    if (!IsValidPosition(tetromino)) {
        gameOver = true;
        std::cout << "Game Over! Final Score: " << score << "\nPress R to restart" << std::endl;
    }
}

void DrawTetromino(const Tetromino& tetromino) {
    for (const auto& block : tetromino.blocks) {
        if (block.first >= 0) {
            DrawBox(block.first, block.second, tetromino.color);
        }
    }
}

void DrawPreviewTetromino(const Tetromino& tetromino) {
    // Calculate the bounding box
    int minRow = INT_MAX, maxRow = INT_MIN;
    int minCol = INT_MAX, maxCol = INT_MIN;

    for (const auto& block : tetromino.blocks) {
        minRow = std::min(minRow, block.first);
        maxRow = std::max(maxRow, block.first);
        minCol = std::min(minCol, block.second);
        maxCol = std::max(maxCol, block.second);
    }

    int tetrominoHeight = maxRow - minRow + 1;
    int tetrominoWidth = maxCol - minCol + 1;

    // Center in preview area
    int offsetX = (4 - tetrominoWidth) / 2;
    int offsetY = (4 - tetrominoHeight) / 2;

    for (const auto& block : tetromino.blocks) {
        int row = block.first - minRow + offsetY;
        int col = block.second - minCol + offsetX;
        DrawPreviewBox(row, col, tetromino.color);
    }
}

// -------------------------------- INPUT HANDLING -------------------------------- //

void ExitProgram(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);  // Close the window
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_LEFT:
                inputState.leftPressed = true;
                break;
            case GLFW_KEY_RIGHT:
                inputState.rightPressed = true;
                break;
            case GLFW_KEY_UP:
                inputState.rotatePressed = true;
                break;
            case GLFW_KEY_SPACE:
                if (!gameOver) {
                    gamePaused = !gamePaused;
                    std::cout << (gamePaused ? "Game Paused" : "Game Resumed") << std::endl;
                }
                break;
            case GLFW_KEY_R:
                if (gameOver) {
                    ResetGame();
                }
                break;
            case GLFW_KEY_E:  // Handle the 'E' key
                std::cout << "\nThank you for playing!! Bye." << std::endl;
                ExitProgram(window);
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_LEFT:
                inputState.leftPressed = false;
                break;
            case GLFW_KEY_RIGHT:
                inputState.rightPressed = false;
                break;
            case GLFW_KEY_UP:
                inputState.rotatePressed = false;
                break;
        }
    }
}

// -------------------------------- INITIALIZATION -------------------------------- //

GLFWwindow* Initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSwapInterval(1);  // Enable vsync

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    // Setup viewport and projection
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(0x1701);
    glLoadIdentity();
    gluOrtho2D(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT);

    glPointSize(1.0f);
    return window;
}

// -------------------------------- MAIN GAME LOOP -------------------------------- //

int main() {
    std::srand(std::time(nullptr));

    GLFWwindow* window = Initialize();
    if (!window) {
        return -1;
    }

    Tetromino currentTetromino = GenerateTetromino();
    Tetromino nextTetromino = GenerateTetromino();
    float fallDelay = INITIAL_FALL_DELAY;
    float lastFallTime = glfwGetTime();

    std::cout << "Welcome to Tetrix!\nScore: 0\nControls:\n"
              << "←/→: Move left/right\n"
              << "↑: Rotate\n"
              << "↓: Fast drop\n"
              << "SPACE: Pause/Resume\n"
              << "R: Restart (when game over)\n"
              << std::endl;

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        double currentTime = glfwGetTime();

        // Draw game over screen if game is over
        if (gameOver) {
            DrawGameOver();
            glfwSwapBuffers(window);
            glfwPollEvents();
            continue;
        }

        // Draw basic game elements
        DrawGrid();
        DrawPreviewArea();
        DrawPreviewTetromino(nextTetromino);

        // Draw locked blocks
        for (int row = 0; row < GRID_ROWS; ++row) {
            for (int col = 0; col < GRID_COLS; ++col) {
                if (grid[row][col].occupied) {
                    DrawBox(row, col, grid[row][col].color);
                }
            }
        }

        if (!gamePaused) {
            // Handle lateral movement with delay
            if (currentTime - inputState.lastMoveTime >= inputState.moveDelay) {
                if (inputState.leftPressed && CanMoveLeft(currentTetromino)) {
                    MoveLeft(currentTetromino);
                    inputState.lastMoveTime = currentTime;
                }
                if (inputState.rightPressed && CanMoveRight(currentTetromino)) {
                    MoveRight(currentTetromino);
                    inputState.lastMoveTime = currentTime;
                }
                if (inputState.rotatePressed) {
                    RotateTetromino(currentTetromino);
                    inputState.rotatePressed = false;
                    inputState.lastMoveTime = currentTime;
                }
            }

            // Handle vertical movement
            if (currentTime - lastFallTime >= fallDelay) {
                if (CanMoveDown(currentTetromino)) {
                    MoveDown(currentTetromino);
                } else {
                    LockTetromino(currentTetromino);
                    ClearLines();
                    currentTetromino = nextTetromino;
                    nextTetromino = GenerateTetromino();
                    CheckGameOver(currentTetromino);
                }
                lastFallTime = currentTime;
            }

            // Handle fast drop
            fallDelay = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                            ? FAST_FALL_DELAY
                            : INITIAL_FALL_DELAY;
        }

        DrawTetromino(currentTetromino);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}