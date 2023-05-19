//
// Created by m4tex on 4/17/23.
//

#include <fstream>
#include <sstream>
#include <iostream>

#include "../include/Shader.h"


Shader::Shader(const std::string &vsPath, const std::string &fsPath)
{
    std::string vs(ParseShaderSource(vsPath)), fs(ParseShaderSource(fsPath));

    m_RendererID = CreateShader(vs, fs);
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath)
{
    std::string vs, fs, gs;
    vs = ParseShaderSource(vsPath);
    fs = ParseShaderSource(fsPath);
    gs = ParseShaderSource(gsPath);

    m_RendererID = CreateShader(vs, fs, gs);
}

Shader::~Shader()
{
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string &name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string &name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4& matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}


int Shader::GetUniformLocation(const std::string &name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1)
        std::cout << "[Warning]: '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}

std::string Shader::ParseShaderSource(const std::string& filepath) {
    std::ifstream stream(filepath);
    if (stream.fail())
        std::cout << "[Error]: Failed to parse the shader, the file could not exist." << std::endl;

    std::stringstream ss;
    ss << stream.rdbuf();

    return ss.str();
}

//ShaderProgramSource Shader::ParseShader(const std::string& filepath)
//{
//    std::ifstream stream(filepath);
//    if (stream.fail())
//        std::cout << "[Error]: Failed to parse the shader, the file could not exist." << std::endl;
//
//    enum class ShaderType {
//        NONE = -1, VERTEX = 0, FRAGMENT = 1
//    };
//
//    std::string line;
//    std::stringstream ss[2];
//
//    ShaderType type = ShaderType::NONE;
//
//    while(getline(stream, line))
//    {
//        if (line.find("#shader") != std::string::npos)
//        {
//            if(line.find("vertex") != std::string::npos)
//                type = ShaderType::VERTEX;
//            else if(line.find("fragment") != std::string::npos)
//                type = ShaderType::FRAGMENT;
//        }
//        else
//        {
//            ss[(int)type] << line << '\n';
//        }
//    }
//
//    return { ss[0].str(), ss[1].str() };
//}


GLuint Shader::CompileShader(GLuint type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        GLchar message[length];

        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to compile a shader!" << std::endl;
        std::cout << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint Shader::CreateShader(const std::string& vShader, const std::string& fShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

GLuint Shader::CreateShader(const std::string& vShader, const std::string& fShader, const std::string& gShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fShader);
    GLuint gs = CompileShader(GL_GEOMETRY_SHADER, gShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glAttachShader(program, gs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(gs);

    return program;
}