//
// Created by m4tex on 4/16/23.
//

#pragma once
#include <iostream>

class IndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const void *data, unsigned int count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    inline unsigned int GetCount() const { std::cout << m_Count << std::endl;
        return m_Count; };
};