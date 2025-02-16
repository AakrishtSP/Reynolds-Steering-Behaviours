// src/Renderer/Shader.h
#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"


struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

class Shader {
    // cache for uniforms
public:
    Shader(std::string  vertexShader, std::string  fragmentShader);
    ~Shader();
    void Bind() const;
    static void Unbind();

    //Set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1iv(const std::string& name, int *value, int count);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);


private:
    static ShaderProgramSource ParseShader(const std::string& vertexShader, const std::string& fragmentShader);
    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    static unsigned int CompileShader(unsigned int type, const std::string& source);
    int GetUniformLocation(const std::string& name);


    unsigned int m_RendererID;
    std::string m_VertexShaderPath;
    std::string m_FragmentShaderPath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};