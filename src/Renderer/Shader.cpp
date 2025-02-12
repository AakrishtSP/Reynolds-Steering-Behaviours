// src/Renderer/Shader.cpp
#include "Shader.h"
#include "glad/gl.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>


Shader::Shader(std::string vertexShader, std::string fragmentShader)
    : m_RendererID(0), m_VertexShaderPath(std::move(vertexShader)), m_FragmentShaderPath(std::move(fragmentShader))
{
    auto [VertexSource, FragmentSource] = ParseShader(m_VertexShaderPath, m_FragmentShaderPath);
    m_RendererID = CreateShader(VertexSource, FragmentSource);
    m_UniformLocationCache.clear();
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::Unbind()
{
    glUseProgram(0);
}


void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1iv(const std::string& name, int* value, int count)
{
    glUniform1iv(GetUniformLocation(name), count, value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (const auto it = m_UniformLocationCache.find(name); it != m_UniformLocationCache.end())
    {
        return it->second;
    }
    const int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
    {
        std::cerr << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    const unsigned int program = glCreateProgram();
    const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    if (vs == 0 || fs == 0)
    {
        std::cerr << "❌ Shader compilation failed. Aborting shader program creation." << std::endl;
        return 0;
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    // Check for linking errors
    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
    {
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetProgramInfoLog(program, length, &length, message.data());

        std::cerr << "❌ Shader program linking failed!" << std::endl;
        std::cerr << message.data() << std::endl;

        glDeleteProgram(program);
        return 0;
    }

    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


ShaderProgramSource Shader::ParseShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    std::ifstream vertexFile(vertexShader, std::ios::in);
    std::ifstream fragmentFile(fragmentShader, std::ios::in);

    if (!vertexFile.is_open() || !fragmentFile.is_open())
    {
        throw std::runtime_error("Failed to open shader files: " + vertexShader + " or " + fragmentShader);
    }

    std::stringstream vertexStream, fragmentStream;
    vertexStream << vertexFile.rdbuf();
    fragmentStream << fragmentFile.rdbuf();

    return {vertexStream.str(), fragmentStream.str()};
}

unsigned int Shader::CompileShader(const unsigned int type, const std::string& source)
{
    const unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message(length);
        glGetShaderInfoLog(id, length, &length, message.data());

        std::cerr << "❌ Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cerr << message.data() << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}
