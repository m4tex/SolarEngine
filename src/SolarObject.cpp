//
// Created by m4tex on 5/19/23.
//

#include "SolarObject.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"

void SolarObject::SetMaterial(Material &newMaterial) {

    _shaderProgram = *new Shader(newMaterial.shaderConfig);

    if (newMaterial.texture != nullptr)
        _shaderProgram.SetUniform1i("u_Texture", 0);

    _material = &newMaterial;
}

void SolarObject::SetModel(Model& model) {
    _model = &model;
}

void SolarObject::PrepareDraw(glm::mat4 povMatrix, LightData lightData) {
    if(_model != nullptr) {
        _model->va.Bind();
        _model->ib.Bind();
    }

    if (_material != nullptr) {
        _shaderProgram.Bind();

        glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);

        glm::vec3 eaRad = eulerAngles * glm::radians(1.0f);

        glm::mat4 rotate = glm::eulerAngleXYZ(eaRad.x, eaRad.y, eaRad.z);
        glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);

        _shaderProgram.SetUniformMat4f("u_MVP", povMatrix * translate * rotate * scaleMat);

        auto color = _material->color;
        _shaderProgram.SetUniform3f("u_Color", color.x, color.y, color.z);
        _shaderProgram.SetUniform1f("u_Bias", lightData.bias);
        auto lDir = lightData.lightDir;
        _shaderProgram.SetUniform3f("u_LightDir", lDir.x, lDir.y, lDir.z);
        auto amb = lightData.ambient;
        _shaderProgram.SetUniform3f("u_Ambient", amb.x, amb.y, amb.z);

        if (_material->texture != nullptr)
            _material->texture->Bind(0);
    }
}

SolarObject::SolarObject() : position({ 0.0f, 0.0f, 0.0f}), eulerAngles({ 0.0f, 0.0f, 0.0f }),
scale({ 1.0f, 1.0f, 1.0f }) {
    _shaderProgram = Shader();
}

SolarObject::SolarObject(Material &material, Model &model) : position({ 0.0f, 0.0f, 0.0f}),
eulerAngles({ 0.0f, 0.0f, 0.0f }), scale({ 1.0f, 1.0f, 1.0f }) {
    SetMaterial(material);
    SetModel(model);
}