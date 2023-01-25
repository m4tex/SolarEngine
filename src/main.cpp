#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
//#include <string>
#include <sstream>
#include <signal.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCALL(x) glClearError(); \
    x;                            \
    ASSERT(glLogCall(#x, __FILE__, __LINE__));

static void glClearError()
{
    while(glGetError() != GL_NO_ERROR);
}

static bool glLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]: Code " << error << " | " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];

    ShaderType type = ShaderType::NONE;

    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static GLuint CompileShader(GLuint type, const std::string& source)
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

static GLuint CreateShader(const std::string& vShader, const std::string fShader)
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

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Poggies", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initiate Glew" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    GLfloat positions[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,
    };

    GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

    ShaderProgramSource shaderProgramSource = ParseShader("../res/shaders/Basic.glsl");
    std::cout << "VERTEX" << std::endl;
    std::cout << shaderProgramSource.vertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << shaderProgramSource.fragmentSource << std::endl;

    GLuint shader = CreateShader(shaderProgramSource.vertexSource, shaderProgramSource.fragmentSource);
    glUseProgram(shader);

    GLuint location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);
    glUniform4f(location, 0.2f, 0.2f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.15f, 0.15f, 0.15f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}