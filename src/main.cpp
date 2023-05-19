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
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Solar Engine", NULL, NULL);
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

//    const GLfloat quadVertices[] = {
//            -1.0f, -1.0f, 0.0f, 0.0f,
//            -1.0f, 1.0f, 0.0f, 1.0f,
//            1.0f, 1.0f, 1.0f, 1.0f,
//            1.0f, -1.0f, 1.0f, 0.0f
//    };
//
//    const GLuint quadIndices[] = {
//            0, 1, 2, 0, 2, 3
//    };

    const GLfloat quadVertices[] = {
            0.0f, 0.0f, 0.0f, 1.0f, // Bottom-left
            0.0f, 1.0f, 0.0f, 0.0f, // Top-left
            1.0f, 1.0f, 1.0f, 0.0f, // Top-right
            1.0f, 0.0f, 1.0f, 1.0f  // Bottom-right
    };

    const GLuint quadIndices[] = {
            0, 1, 2, 2, 3, 0
    };

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

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 512, 512, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GLuint depthBuffer;
    glGenFramebuffers(1, &depthBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMap);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "[Error]: Depth framebuffer incomplete." << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    VertexArray va;
    VertexBuffer vb(positions, 4 * 5 * sizeof(float));

    VertexBufferLayout layout;
    layout.PushF(3);
    layout.PushF(2);
    va.AddBuffer(vb, layout);

    vb.Unbind();

    IndexBuffer ib(indices, 6); //Gets bound to the va automatically

    Shader shader("../res/shaders/basic/textured_vertex.glsl",
                  "../res/shaders/basic/textured_lit_fragment.glsl",
                  "../res/shaders/basic/textured_lit_geometry.glsl");

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


    float translateX = 0.0f, translateY = 0.0f, translateZ = -15.0f;
    float rotationX = 0.0f, rotationY = 0.0f, rotationZ = 0.0f;
    float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
    bool autoSpin = true;
    float spinAmount = 0.2f;

    float lightBias = .25f;
    float lightDirX = 1.0f, lightDirY = -0.3f, lightDirZ = 2.0f;
    float ambientR = .05f, ambientG = .05f, ambientB = .05f;

    float fov = 90.0f;
    bool showZBuffer = false;

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        glm::mat4 proj = glm::perspective(glm::radians(fov), 1.0f, 0.001f, 1000.0f);

        glm::mat4 view = glm::mat4(1.0f);

        glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(translateX, translateY, translateZ));
        glm::mat4 rotate = glm::eulerAngleXYZ(glm::radians(rotationX), glm::radians(rotationY), glm::radians(rotationZ));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, scaleZ));

        glm::mat4 mvp = proj * view * translate * rotate * scale;

        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        shader.SetUniform3f("u_LightDir", lightDirX, lightDirY, lightDirZ);
        shader.SetUniform3f("u_Ambient", ambientR, ambientG, ambientB);
        shader.SetUniform1f("u_Bias", lightBias);

        renderer.Draw(va, ib, shader);

        if (autoSpin) rotationY += spinAmount;
        if (autoSpin && rotationY >= 359.9f) rotationY = 0.0f;

        if (showZBuffer) {
            VertexArray quadVa;
            VertexBuffer quadVb(quadVertices, 4 * 4 * sizeof(float));

            VertexBufferLayout vbLayout;
            vbLayout.PushF(2);
            vbLayout.PushF(2);

            quadVa.AddBuffer(quadVb, vbLayout);
            quadVb.Unbind();

            IndexBuffer quadIbo(quadIndices, 6);

            Shader depthShader("../res/shaders/extra/depth_map_vertex.glsl",
                               "../res/shaders/extra/depth_map_fragment.glsl");

            depthShader.Bind();
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            depthShader.SetUniform1i("u_DepthMap", 1);

            renderer.Draw(quadVa, quadIbo, Shader("../res/shader/extra/untransformed_vertex.glsl",
                                                  "../res/shader/basic/textured_unlit_fragment.glsl"));
        }

        {
            ImGui::Begin("Model");
            ImGui::Text("Position");
            ImGui::SliderFloat("PosX", &translateX, -50.0f, 50.0f);
            ImGui::SliderFloat("PosY", &translateY, -50.0f, 50.0f);
            ImGui::SliderFloat("PosZ", &translateZ, -50.0f, 50.0f);
            ImGui::Text("Rotation");
            ImGui::Checkbox("Spin", &autoSpin);
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
            ImGui::InputFloat("Bias", &lightBias);
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

        {
            ImGui::Begin("View");
            ImGui::Checkbox("Show Z-Buffer", &showZBuffer);
            ImGui::SliderFloat("FOV", &fov, 1.0f, 179.0f);
            ImGui::Text("Bloom Parameters");
//            ImGui::SliderFloat("Intensity");
            ImGui::End();
        } //Window drawing

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}

