// src/Renderer/VertexBuffer.cpp
#include "VertexBuffer.h"
#include "glad/gl.h"
#include "pch.h"

unsigned int VertexBuffer::s_CurrentlyBound=0;

VertexBuffer::VertexBuffer(const void* data, const unsigned int size, const bool isDynamic) :m_bIsDynamic(isDynamic), m_Size(size)
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    if (isDynamic)
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }
    s_CurrentlyBound = m_RendererID;
}

VertexBuffer::VertexBuffer(const unsigned int size) :m_bIsDynamic(true), m_Size(size)
{
    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    s_CurrentlyBound = m_RendererID;
}

VertexBuffer::~VertexBuffer()
{
    Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::SetData(const void* data, const unsigned int size, const bool resize/*=false*/)
{
    if (!m_bIsDynamic)
        std::cerr << "Warning: Trying to edit a static VertexBuffer!: "<< m_RendererID << std::endl;

    Bind();
    if (resize || size>m_Size )
    {
        m_Size = size;
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}

void VertexBuffer::EditData(const void* data, const unsigned int size, const unsigned int offset/*=0*/) const
{
    if (!m_bIsDynamic)
        std::cerr << "Warning: Trying to edit a static VertexBuffer!: " << m_RendererID << std::endl;

    Bind();
    if (offset + size > m_Size)
    {
        std::cerr << "Warning: Trying to edit a VertexBuffer out of bounds!: " << m_RendererID << std::endl;
        assert(false);
    }
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Bind() const
{
    if (s_CurrentlyBound == m_RendererID)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    s_CurrentlyBound = m_RendererID;
}

void VertexBuffer::Unbind()
{
    if (s_CurrentlyBound == 0)
        return;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    s_CurrentlyBound = 0;
}