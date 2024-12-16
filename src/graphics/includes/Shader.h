#pragma once

#include <GL/glew.h>

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

/**
 * @struct ShaderProgramSource
 * @brief Stores the vertex and fragment shader source code.
 */
struct ShaderProgramSource {
    std::string VertexSource;    // Source code for the vertex shader.
    std::string FragmentSource;  // Source code for the fragment shader.
};

/**
 * @class Shader
 * @brief Encapsulates the functionality of an OpenGL Shader Program.
 */
class Shader {
   private:
    std::string m_FilePath;                                       // Filepath to the shader source.
    unsigned int m_RendererID;                                    // OpenGL ID for the shader program.
    std::unordered_map<std::string, int> m_UniformLocationCache;  // Cache for uniform locations.

    /**
     * @brief Parses the shader file and separates vertex and fragment shader code.
     *
     * @param file Path to the shader source file.
     * @return A ShaderProgramSource struct containing the separated shader code.
     */
    ShaderProgramSource ParseShader(const std::string& file);

    /**
     * @brief Creates and links a shader program.
     *
     * @param vertexShader Vertex shader source code.
     * @param fragmentShader Fragment shader source code.
     * @return The OpenGL ID of the created shader program.
     */
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    /**
     * @brief Compiles an individual shader.
     *
     * @param type The type of the shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
     * @param source The source code of the shader.
     * @return The OpenGL ID of the compiled shader.
     */
    unsigned int CompileShader(unsigned int type, const std::string& source);

    /**
     * @brief Retrieves the location of a uniform variable.
     *
     * Uses a cache to optimize uniform lookup.
     *
     * @param name Name of the uniform variable.
     * @return The location of the uniform in the shader program.
     */
    unsigned int GetUniformLocation(const std::string& name);

   public:
    /**
     * @brief Constructs a Shader from a file.
     *
     * @param filepath Path to the shader source file containing both vertex and fragment shaders.
     */
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    /**
     * @brief Sets an integer uniform variable in the shader.
     *
     * @param name Name of the uniform variable.
     * @param value Integer value to set.
     */
    void SetUniform1i(const std::string& name, int value);

    /**
     * @brief Sets a 4-component float uniform variable in the shader.
     *
     * @param name Name of the uniform variable.
     * @param v0 First float value.
     * @param v1 Second float value.
     * @param v2 Third float value.
     * @param v3 Fourth float value.
     */
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

    /**
     * @brief Sets a 4x4 matrix uniform variable in the shader.
     *
     * @param name Name of the uniform variable.
     * @param matrix The 4x4 matrix to set (in glm::mat4 format).
     */
    void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
};
