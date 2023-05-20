//
// Created by m4tex on 5/19/23.
//

#pragma once

#include <glm/glm.hpp>
#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include <memory>

struct LightData {
    glm::vec3 lightDir;
    glm::vec3 ambient;
    float bias;
};

struct Model {
    std::unique_ptr<VertexArray> va;
    std::unique_ptr<IndexBuffer> ib;
    std::unique_ptr<VertexBuffer> vb;

    Model (const std::string& filepath) {
        LoadFromFile(filepath);
    }

    void LoadFromFile(const std::string& filepath);
};

const ShaderConfig defaultShaderConfig = {
        "../res/shaders/standard/basic_vertex.glsl",
        "../res/shaders/standard/basic_lit_fragment.glsl",
        "../res/shaders/standard/basic_lit_geometry.glsl"
};

struct Material {
    glm::vec3 color = { 1.0f, 1.0f, 1.0f };
    float shininess = 0.5f;
    Texture* texture = nullptr;
    ShaderConfig shaderConfig = defaultShaderConfig;
};

class SolarObject {
public:
    glm::vec3 position;
    glm::vec3 eulerAngles;
    glm::vec3 scale;

    SolarObject();
    SolarObject(Material& material, Model& model);

    void SetMaterial(Material& material);
    void SetModel(Model& model);
    void PrepareDraw(glm::mat4 perspective, glm::mat4 view, LightData lightData);

    Model* model = nullptr;
private:
    Material* _material = nullptr;
    Shader _shaderProgram;
};
