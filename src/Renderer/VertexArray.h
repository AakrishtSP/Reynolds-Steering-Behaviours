// src/Renderer/VertexArray.h
#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const;
    void Bind() const;
    static void Unbind();
private:
    unsigned int m_RendererID{};
};