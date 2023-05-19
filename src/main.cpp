#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void GLAPIENTRY MessageCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar* message, const void*) {
    std::cout << "[GL Message]: " << message << std::endl;
}

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);


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

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);

    std::cout << glGetString(GL_VERSION) << std::endl;

    GLfloat positions[] = {
            -10.0f, -10.0f, 0.0f, 0.0f, 0.0f,
            10.0f, -10.0f, 0.0f, 1.0f, 0.0f,
            10.0f, 10.0f, 0.0f, 1.0f, 1.0f,
            -10.0f, 10.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexArray va;
    VertexBuffer vb(positions, 4 * 5 * sizeof(float));

    VertexBufferLayout layout;
    layout.PushF(3);
    layout.PushF(2);
    va.AddBuffer(vb, layout);

    vb.Unbind();

    IndexBuffer ib(indices, 6); //Gets bound to the va automatically

//    glm::vec4 v1 = mvp * glm::vec4(positions[0], positions[1], positions[2], 1.0f);
//    glm::vec4 v2 = mvp * glm::vec4(positions[5], positions[6], positions[7], 1.0f);
//    glm::vec4 v3 = mvp * glm::vec4(positions[10], positions[11], positions[12], 1.0f);
//    glm::vec4 v4 = mvp * glm::vec4(positions[15], positions[16], positions[17], 1.0f);
//
//    v1 /= v1.w;
//    v2 /= v2.w;
//    v3 /= v3.w;
//    v4 /= v4.w;
//
//    std::cout << std::to_string(v1.x) + ' ' + std::to_string(v1.y) + ' ' + std::to_string(v1.z) << std::endl;
//    std::cout << std::to_string(v2.x) + ' ' + std::to_string(v2.y) + ' ' + std::to_string(v2.z) << std::endl;
//    std::cout << std::to_string(v3.x) + ' ' + std::to_string(v3.y) + ' ' + std::to_string(v3.z) << std::endl;
//    std::cout << std::to_string(v4.x) + ' ' + std::to_string(v4.y) + ' ' + std::to_string(v4.z) << std::endl;

    Shader shader("../res/shaders/basic/basic_vertex.glsl",
                  "../res/shaders/basic/basic_fragment.glsl",
                  "../res/shaders/basic/basic_geometry.glsl");

    Texture texture("../res/textures/texture.jpg");
    texture.Bind();

    shader.Bind();
    shader.SetUniform1i("u_Texture", 0);

    Renderer renderer;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsDark();

    glClearColor(0.15f, 0.15f, 0.15f, 1);

    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.001f, 1000.0f);

    float translateX = 0.0f, translateY = 0.0f, translateZ = -5.0f;
    float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
    float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;

    float lightDirX = 1.0f, lightDirY = -0.3f, lightDirZ = 0.0f;
    float ambientR = .2f, ambientG = .2f, ambientB = .2f;

    //For now the origin will be 0, 0, 0
//    glm::vec3 origin

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        glm::mat4 view = glm::mat4(1.0f);

        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(translateX, translateY, translateZ));
        glm::mat4 rotate = glm::eulerAngleXYZ(glm::radians(rotationX), glm::radians(rotationY), glm::radians(rotationZ));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));

        glm::mat4 mvp = proj * view * translate * rotate * scale;

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniform3f("u_LightDir", lightDirX, lightDirY, lightDirZ);
        shader.SetUniform3f("u_Ambient", ambientR, ambientG, ambientB);

        renderer.Draw(va, ib, shader);

        {
            ImGui::Begin("Model");
            ImGui::Text("Position");
            ImGui::SliderFloat("PosX", &translateX, -50.0f, 50.0f);
            ImGui::SliderFloat("PosY", &translateY, -50.0f, 50.0f);
            ImGui::SliderFloat("PosZ", &translateZ, -50.0f, 50.0f);
            ImGui::Text("Rotation");
            ImGui::SliderFloat("RotX", &rotationX, 0.0f, 360.0f);
            ImGui::SliderFloat("RotY", &rotationY, 0.0f, 360.0f);
            ImGui::SliderFloat("RotZ", &rotationZ, 0.0f, 360.0f);
            ImGui::Text("Scale");
            ImGui::SliderFloat("ScaleX", &scaleX, 0.0f, 20.0f);
            ImGui::SliderFloat("ScaleY", &scaleY, 0.0f, 20.0f);
            ImGui::SliderFloat("ScaleZ", &scaleZ, 0.0f, 20.0f);
            ImGui::End();
        }

        {
            ImGui::Begin("Lighting");
            ImGui::Text("Light Direction");
            ImGui::SliderFloat("LightDirX", &lightDirX, -2.0f, 2.0f);
            ImGui::SliderFloat("LightDirY", &lightDirY, -2.0f, 2.0f);
            ImGui::SliderFloat("LightDirZ", &lightDirZ, -2.0f, 2.0f);
            ImGui::Text("Ambient");
            ImGui::SliderFloat("R", &ambientR, 0.0f, 1.0f);
            ImGui::SliderFloat("G", &ambientG, 0.0f, 1.0f);
            ImGui::SliderFloat("B", &ambientB, 0.0f, 1.0f);
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}