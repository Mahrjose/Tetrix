#pragma once

#include <GL/glew.h>

#include <iostream>
#include <string>

// Define APIENTRY if not already defined
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#ifndef APIENTRY
#define APIENTRY
#endif
#endif

class ErrorHandler {
   public:
    static void EnableDebugOutput();
    static void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                       GLsizei length, const GLchar* message, const void* userParam);
};
