#pragma once

#include <GL/glew.h>

#include <string>
#include <unordered_map>

#include "../lib/glm/glm.hpp"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
   private:
    std::string m_FilePath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    ShaderProgramSource ParseShader(const std::string& file);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int GetUniformLocation(const std::string& name);

   public:
    Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
    void SetUniformMat4f(const std::string& name, const glm::mat4 matrix);
};
