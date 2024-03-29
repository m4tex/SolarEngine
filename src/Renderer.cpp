//
// Created by m4tex on 4/16/23.
//

#include "../include/Renderer.h"

void Renderer::Draw(Camera& camera, SolarObject& object) const {
    object.PrepareDraw(camera.PerspectiveMatrix(), camera.ViewMatrix(), { lightDir, ambient, bias });

    glDrawElements(GL_TRIANGLES, object.model->ib->GetCount(), GL_UNSIGNED_INT, nullptr);
}