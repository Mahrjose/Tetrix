// #include <GL/glew.h>
// #include <GLFW/glfw3.h>

// #include <algorithm>
// #include <cmath>
// #include <iostream>
// #include <vector>

// #include "ErrorHandler.h"
// #include "Grid.h"
// #include "Renderer.h"
// #include "Tetromino.h"
// #include "VertexBuffer.h"

// bool isKeyPressed(GLFWwindow* window, int key) {
//     return glfwGetKey(window, key) == GLFW_PRESS;
// }

// GLFWwindow* Initialize() {
//     if (!glfwInit()) {
//         std::cerr << "Failed to initialize GLFW!" << std::endl;
//         return nullptr;
//     };

//     // Create a GLFW window
//     GLFWwindow* window = glfwCreateWindow(683, 738, "Orb Invader", NULL, NULL);
//     if (!window) {
//         std::cerr << "Failed to create GLFW window!" << std::endl;
//         glfwTerminate();
//         return nullptr;
//     }
//     glfwMakeContextCurrent(window);
//     glfwSwapInterval(1);  // Enable vsync

//     if (glewInit() != GLEW_OK) {
//         std::cerr << "Failed to initialize GLEW!" << std::endl;
//         glfwDestroyWindow(window);
//         glfwTerminate();
//         return nullptr;
//     }
//     // glViewport(0, 0, 683, 738);
//     std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

//     return window;
// }

// int main(void) {
//     GLFWwindow* window = Initialize();
//     if (!window) {
//         return -1;
//     }

//     // Enable OpenGL ErrorHandling
//     ErrorHandler errorHandler;
//     errorHandler.EnableDebugOutput();

//     Grid grid;
//     Tetromino tetromino;
//     tetromino.SetShape(20, 0, ShapeType::L);

//     Renderer renderer;

//     double lastTime = glfwGetTime();
//     double fallInterval = 1;  // Tetromino falls every 0.5 seconds

//     while (!glfwWindowShouldClose(window)) {
//         double currentTime = glfwGetTime();
//         double deltaTime = currentTime - lastTime;

//         renderer.ClearScreen();

//         if (deltaTime >= fallInterval) {
//             if (grid.CanMoveTetromino(tetromino, true, false, false)) {
//                 tetromino.MoveDown();
//             } else {
//                 grid.PlaceTetromino(tetromino);
//             }
//             lastTime = currentTime;
//         }

//         // Handle user input
//         if (isKeyPressed(window, GLFW_KEY_LEFT) && grid.CanMoveTetromino(tetromino, false, true, false)) {
//             tetromino.MoveLeft();
//         } else if (isKeyPressed(window, GLFW_KEY_RIGHT) && grid.CanMoveTetromino(tetromino, false, false, true)) {
//             tetromino.MoveRight();
//         } else if (isKeyPressed(window, GLFW_KEY_DOWN) && grid.CanMoveTetromino(tetromino, true, false, false)) {
//             tetromino.MoveDown();
//         }

//         tetromino.Draw();
//         grid.Draw();

//         glfwSwapBuffers(window);
//         glfwPollEvents();
//     }

//     glfwTerminate();
//     return 0;
// }

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "ErrorHandler.h"
#include "Grid.h"
#include "Renderer.h"
#include "Tetromino.h"
#include "VertexBuffer.h"

bool isKeyPressed(GLFWwindow* window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

#include <cstdlib>  // For std::rand and std::srand
#include <ctime>    // For std::time

Tetromino SpawnRandomTetromino() {
    // Seed the random number generator
    std::srand(static_cast<unsigned int>(std::time(0)));

    ShapeType shapes[] = {ShapeType::I, ShapeType::L, ShapeType::O};
    ShapeType randomShape = shapes[std::rand() % 3];

    int randomCol = std::rand() % 10;

    // Create and return a new Tetromino with the random shape and position
    Tetromino tetromino;
    tetromino.SetShape(20, randomCol, randomShape);
    return tetromino;
}

GLFWwindow* Initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return nullptr;
    };

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(683, 738, "Orb Invader", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW!" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }
    // glViewport(0, 0, 683, 738);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    return window;
}

int main(void) {
    GLFWwindow* window = Initialize();
    if (!window) {
        return -1;
    }

    // Enable OpenGL ErrorHandling
    ErrorHandler errorHandler;
    errorHandler.EnableDebugOutput();

    Grid grid;
    Tetromino tetromino = SpawnRandomTetromino();

    Renderer renderer;

    double lastTime = glfwGetTime();
    double fallInterval = 1;  // Tetromino falls every 0.5 seconds

    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        renderer.ClearScreen();
        grid.Draw();
        tetromino.Draw();

        if (deltaTime >= fallInterval) {
            if (grid.CanMoveTetromino(tetromino, true, false, false)) {
                tetromino.MoveDown();
            } else {
                grid.PlaceTetromino(tetromino);
                tetromino = SpawnRandomTetromino();
            }
            lastTime = currentTime;
        }

        // Handle user input
        if (isKeyPressed(window, GLFW_KEY_LEFT) && grid.CanMoveTetromino(tetromino, false, true, false)) {
            tetromino.MoveLeft();
        } else if (isKeyPressed(window, GLFW_KEY_RIGHT) && grid.CanMoveTetromino(tetromino, false, false, true)) {
            tetromino.MoveRight();
        } else if (isKeyPressed(window, GLFW_KEY_DOWN) && grid.CanMoveTetromino(tetromino, true, false, false)) {
            tetromino.MoveDown();
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
