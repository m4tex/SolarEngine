//
// Created by m4tex on 4/16/23.
//

#include "../include/Renderer.h"

void Renderer::Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const {
    shader.Bind();

    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
