#define WINDOW_WIDTH 512
#define WINDOW_HEIGHT 512

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void GLAPIENTRY MessageCallback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar* message, const void*) {
    std::cout << "[GL Message]: " << message << std::endl;
}

void key_function(GLFWwindow* window, int key, int, int action, int) {
    if (action == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

}

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

//    glEnable(GL_DEBUG_OUTPUT);
//    glDebugMessageCallback(MessageCallback, nullptr);

    glfwSetKeyCallback(window, key_function);

    std::cout << glGetString(GL_VERSION) << std::endl;

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    ImGui::StyleColorsDark();

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

    VertexArray va;
    VertexBuffer vb(positions, 4 * 5 * sizeof(float));

    VertexBufferLayout layout;
    layout.PushF(3);
    layout.PushF(2);

    va.AddBuffer(vb, layout);
    vb.Unbind();

    IndexBuffer ib(indices, 6);

    Model planeModel = { va, ib };
    Model planeModel2 = { va, ib };
    Material material;
    material.texture = new Texture("../res/textures/texture.jpg");
    material.shaderConfig.vertexSource = "../res/shaders/standard/textured_vertex.glsl";
    material.shaderConfig.fragmentSource = "../res/shaders/standard/textured_lit_fragment.glsl";
    material.shaderConfig.geometrySource = "../res/shaders/standard/textured_lit_geometry.glsl";

    Renderer renderer;

    Camera camera { glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), 90.0f, 0.001f, 1000.0f };

    SolarObject stefan(material, planeModel);
    stefan.position.z = -15.0f;
    SolarObject stefan2(material, planeModel2);
    stefan2.position.z = -15.0f;

    bool autoSpin = true;
    float spinAmount = 0.2f;

    glClearColor(0.15f, 0.15f, 0.15f, 1);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        renderer.Draw(camera, stefan);
        renderer.Draw(camera, stefan2);

        if (autoSpin) stefan.eulerAngles.y += spinAmount;
        if (autoSpin && stefan.eulerAngles.y >= 359.9f) stefan.eulerAngles.y = 0.0f;

        {
            ImGui::Begin("Model");
            ImGui::Text("Position");
            ImGui::SliderFloat("PosX", &stefan.position.x, -50.0f, 50.0f);
            ImGui::SliderFloat("PosY", &stefan.position.y, -50.0f, 50.0f);
            ImGui::SliderFloat("PosZ", &stefan.position.z, -50.0f, 50.0f);
            ImGui::Text("Rotation");
            ImGui::Checkbox("Spin", &autoSpin);
            ImGui::SliderFloat("RotX", &stefan.eulerAngles.x, 0.0f, 360.0f);
            ImGui::SliderFloat("RotY", &stefan.eulerAngles.y, 0.0f, 360.0f);
            ImGui::SliderFloat("RotZ", &stefan.eulerAngles.z, 0.0f, 360.0f);
            ImGui::Text("Scale");
            ImGui::SliderFloat("ScaleX", &stefan.scale.x, 0.0f, 20.0f);
            ImGui::SliderFloat("ScaleY", &stefan.scale.y, 0.0f, 20.0f);
            ImGui::SliderFloat("ScaleZ", &stefan.scale.z, 0.0f, 20.0f);
            ImGui::End();
        }

        {
            ImGui::Begin("Lighting");
            ImGui::InputFloat("Bias", &renderer.bias);
            ImGui::Text("Light Direction");
            ImGui::SliderFloat("LightDirX", &renderer.lightDir.x, -2.0f, 2.0f);
            ImGui::SliderFloat("LightDirY", &renderer.lightDir.y, -2.0f, 2.0f);
            ImGui::SliderFloat("LightDirZ", &renderer.lightDir.z, -2.0f, 2.0f);
            ImGui::Text("Ambient");
            ImGui::SliderFloat("R", &renderer.ambient.r, 0.0f, 1.0f);
            ImGui::SliderFloat("G", &renderer.ambient.g, 0.0f, 1.0f);
            ImGui::SliderFloat("B", &renderer.ambient.b, 0.0f, 1.0f);
            ImGui::End();
        }

        {
            ImGui::Begin("View");
            ImGui::SliderFloat("FOV", &camera.fov, 1.0f, 179.0f);
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