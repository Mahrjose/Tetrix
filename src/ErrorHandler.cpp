#include "includes/ErrorHandler.h"

void ErrorHandler::EnableDebugOutput() {
    glEnable(GL_DEBUG_OUTPUT);                                                             // Enable OpenGL debug output
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);                                                 // Makes sure the messages are synchronous
    glDebugMessageCallback(DebugCallback, nullptr);                                        // Register the callback function
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);  // Enable all messages
}

void APIENTRY ErrorHandler::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                          GLsizei length, const GLchar* message, const void* userParam) {
    // Ignore insignificant non-error messages
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) {
        return;
    }

    std::string sourceStr;
    switch (source) {
        case GL_DEBUG_SOURCE_API:
            sourceStr = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceStr = "Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceStr = "Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceStr = "Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            sourceStr = "Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            sourceStr = "Other";
            break;
    }

    std::string typeStr;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typeStr = "Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeStr = "Deprecated Behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeStr = "Undefined Behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeStr = "Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeStr = "Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            typeStr = "Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            typeStr = "Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            typeStr = "Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeStr = "Other";
            break;
    }

    std::string severityStr;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            severityStr = "High";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severityStr = "Medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severityStr = "Low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            severityStr = "Notification";
            break;
    }

    // Log the error message
    std::cerr << "\n=>[OpenGL Debug] "
              << "Source: " << sourceStr << ", "
              << "Type: " << typeStr << ", "
              << "ID: " << id << ", "
              << "Severity: " << severityStr << "\n"
              << "Message: " << message << std::endl;
}
