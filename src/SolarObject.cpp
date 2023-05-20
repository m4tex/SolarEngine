//
// Created by m4tex on 5/19/23.
//

#include "SolarObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

void SolarObject::SetMaterial(Material &newMaterial) {

    _shaderProgram = *new Shader(newMaterial.shaderConfig);

    if (newMaterial.texture != nullptr)
        _shaderProgram.SetUniform1i("u_Texture", 0);

    _material = &newMaterial;
}

void SolarObject::SetModel(Model& model) {
    this->model = &model;
}

void SolarObject::PrepareDraw(glm::mat4 perspectiveMatrix, glm::mat4 viewMatrix, LightData lightData) {
    if(model != nullptr) {
        model->va->Bind();
        model->ib->Bind();
    }

    if (_material != nullptr) {
        _shaderProgram.Bind();

        glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);

        glm::vec3 eaRad = eulerAngles * glm::radians(1.0f);

        glm::mat4 rotate = glm::eulerAngleXYZ(eaRad.x, eaRad.y, eaRad.z);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

        glm::mat4 modelMatrix = translate * rotate * scaleMat;

        _shaderProgram.SetUniformMat4f("u_Model", modelMatrix);
        _shaderProgram.SetUniformMat4f("u_ViewPerspective", perspectiveMatrix * viewMatrix);

        auto color = _material->color;
        _shaderProgram.SetUniform3f("u_Color", color.x, color.y, color.z);

        _shaderProgram.SetUniform1f("u_Shininess", _material->shininess);

        _shaderProgram.SetUniform1f("u_Bias", lightData.bias);

        auto lDir = lightData.lightDir;
        _shaderProgram.SetUniform3f("u_LightDir", lDir.x, lDir.y, lDir.z);

        auto amb = lightData.ambient;
        _shaderProgram.SetUniform3f("u_Ambient", amb.x, amb.y, amb.z);

        if (_material->texture != nullptr)
            _material->texture->Bind(0);
    }
}

SolarObject::SolarObject() : position({ 0.0f, 0.0f, 0.0f }),
eulerAngles({ 0.0f, 0.0f, 0.0f }), scale({ 1.0f, 1.0f, 1.0f }) {}

SolarObject::SolarObject(Material &material, Model &model) : position({ 0.0f, 0.0f, 0.0f}),
eulerAngles({ 0.0f, 0.0f, 0.0f }), scale({ 1.0f, 1.0f, 1.0f }) {
    SetMaterial(material);
    SetModel(model);
}

void Model::LoadFromFile(const std::string& filepath) {
    std::vector<float> vertices;
    std::vector<GLuint> indices;

    std::ifstream file(filepath);

    if(file.fail()) std::cout << "[Error]: Failed to load in a model from file." << std::endl;

    std::string line;
    while (std::getline(file, line)) {
        if (line[0] == 'v') {
            std::istringstream s(line.substr(2));
            float x, y, z;

            s >> x;
            s >> y;
            s >> z;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if (line[0] == 'f') {
            std::istringstream s(line.substr(2));
            unsigned int i, j, k;

            s >> i;
            s >> j;
            s >> k;

            indices.push_back(i-1);
            indices.push_back(j-1);
            indices.push_back(k-1);
        }
    }

    va = std::make_unique<VertexArray>();

    vb = std::make_unique<VertexBuffer>(vertices.data(),
                                             vertices.size() * sizeof(float));

    ib = std::make_unique<IndexBuffer>(indices.data(), indices.size());

    VertexBufferLayout layout;
    layout.PushF(3);

    va->AddBuffer(*vb, layout);

    vb->Unbind();
}