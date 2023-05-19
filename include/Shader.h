//
// Created by m4tex on 4/17/23.
//

#pragma once
#include <string>
#include <unordered_map>
#include "Debugging.h"
#include "glm/glm.hpp"

typedef unsigned int GLuint;

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

class Shader {
private:
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

public:
    Shader(const std::string& vsPath, const std::string& fsPath);
    Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
//    ShaderProgramSource ParseShader(const std::string& filepath);
    std::string ParseShaderSource(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& gShader);
    int GetUniformLocation(const std::string& name);
};