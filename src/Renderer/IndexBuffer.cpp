// src/Renderer/IndexBuffer.cpp
#include "IndexBuffer.h"
#include "glad/gl.h"
#include "pch.h"

#include <iostream>

unsigned int IndexBuffer::s_CurrentlyBound = 0;

IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count, const bool isDynamic/* = false*/)
    : m_Count(count), m_BufferSize(count * sizeof(unsigned int)), m_bIsDynamic(isDynamic)
{
    assert(sizeof(unsigned int) == sizeof(GLuint));
    assert(data != nullptr && count != 0);


    glGenBuffers(1, &m_RendererID);

    Bind();
    if (isDynamic)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferSize, data, GL_DYNAMIC_DRAW);
    }
    else
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferSize, data, GL_STATIC_DRAW);
    }
}

IndexBuffer::IndexBuffer(const unsigned int count)
    : m_Count(count), m_BufferSize(count * sizeof(unsigned int)), m_bIsDynamic(true)
{
    assert(sizeof(unsigned int) == sizeof(GLuint));

    glGenBuffers(1, &m_RendererID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferSize, nullptr, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    Unbind();
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::EditData(const unsigned int* data, const unsigned int count, const unsigned int offset /*= 0*/)
{
    if (!m_bIsDynamic)
        std::cerr << "Warning: Trying to edit a static IndexBuffer!: " << m_RendererID << std::endl;

    assert(data != nullptr && count == 0);
    Bind();

    const unsigned int newCount = std::max(count + offset, m_Count);

    if (const unsigned int newBufferSize = newCount * sizeof(unsigned int); newBufferSize > m_BufferSize)
    {
        const unsigned int oldCount = m_Count;
        m_Count = newCount;
        m_BufferSize = newBufferSize;

        // if offset is positive, we need to copy the old data back
        if (offset > 0)
        {
            auto* tempData = new unsigned int[oldCount];
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<long>(sizeof(unsigned int)) * oldCount, tempData);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferSize, nullptr, GL_DYNAMIC_DRAW);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(sizeof(unsigned int)) * offset, m_BufferSize, tempData);
            // Copy old data back
            delete[] tempData;
        }
        // if offset is 0, we need to copy the new data to the front
        else
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BufferSize, nullptr, GL_DYNAMIC_DRAW);
        }
    }
    else // if the new data fits in the buffer copy it directly
    {
        m_Count = std::max(count + offset, m_Count);
        assert(m_Count * sizeof(unsigned int) <= m_BufferSize);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(offset * sizeof(unsigned int)), static_cast<long>(count * sizeof(unsigned int)), data);
    }
}

void IndexBuffer::GenerateQuadDefault(const unsigned int count, const unsigned int offsetQuad /* = 0*/)
{
    /*
        pattern:
        0,1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    */
    const unsigned int offset = offsetQuad * 4;

    // 'count' refers to the number of quads, not the number of indices.
    auto* data = new unsigned int[count * 6];

    for (unsigned int i = 0; i < count; i += 1)
    {
        data[i * 6] = i * 4 + offset;
        data[i * 6 + 1] = i * 4 + 1 + offset;
        data[i * 6 + 2] = i * 4 + 2 + offset;
        data[i * 6 + 3] = i * 4 + 2 + offset;
        data[i * 6 + 4] = i * 4 + 3 + offset;
        data[i * 6 + 5] = i * 4 + offset;
    }
    EditData(data, count * 6, 0);
    delete[] data;
}


void IndexBuffer::Bind() const
{
    if (s_CurrentlyBound == m_RendererID)
        return;
    (glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    s_CurrentlyBound = m_RendererID;
}

void IndexBuffer::Unbind()
{
    if (s_CurrentlyBound == 0)
        return;
    (glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    s_CurrentlyBound = 0;
}
