//
// Created by m4tex on 4/16/23.
//

#pragma once

#include <csignal>
#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Debugging.h"
#include "SolarObject.h"
#include "glm/gtc/matrix_transform.hpp"

struct Camera {
    glm::vec3 position;
    glm::vec2 rotation; // No rolling rotation
    float fov, zNear, zFar;

    glm::mat4 PerspectiveViewMatrix() const {
        glm::vec3 rotRad = glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), 0.0f);
        glm::vec3 lookingDir = { glm::cos(rotRad.x) * glm::sin(rotRad.y),
                                 glm::sin(rotRad.x),
                                 glm::cos(rotRad.x) * glm::cos(rotRad.y)};

        auto viewMat = glm::lookAt(position, position + rotRad,glm::vec3(0.0f, 1.0f, 0.0f));
        viewMat = glm::mat4(1.0f);
        return glm::perspective(glm::radians(fov), 1.0f, zNear, zFar) * viewMat;
    }
};


class Renderer {
public:
    glm::vec3 lightDir = { 0.0f, 0.0f, 2.0f };
    glm::vec3 ambient = { .1f, .1f, .1f };
    float bias = .25f;

//    void Draw(Camera& camera, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;

    void Draw(Camera& camera, SolarObject& object) const;
};