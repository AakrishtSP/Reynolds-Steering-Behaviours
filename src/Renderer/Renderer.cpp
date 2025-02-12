// src/Renderer/Renderer.cpp
#include "Renderer.h"
#include "glad/gl.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<int>(ib.GetCount()), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::SetClearColor(const float r, const float g, const float b, const float a)
{
    glClearColor(r, g, b, a);
}
