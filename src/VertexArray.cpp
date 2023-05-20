//
// Created by m4tex on 4/16/23.
//

#include "../include/VertexArray.h"
#include "../include/VertexBufferLayout.h"
#include "../include/Renderer.h"
#include "iostream"

VertexArray::VertexArray() {
    std::cout << "construct " << m_RendererID << std::endl;
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
    std::cout << "destruct " << m_RendererID << std::endl;
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) {
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }

}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}
