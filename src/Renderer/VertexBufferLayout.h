// src/Renderer/VertexBufferLayout.h
#pragma once

#include "glad/gl.h"
#include "pch.h"

    struct VertexBufferElement
    {
        unsigned int type;
        unsigned int count;
        unsigned int offset;
        unsigned char normalized;

        static unsigned int GetSizeOfType(const unsigned int type)
        {
            switch (type)
            {
            case GL_FLOAT:
            case GL_UNSIGNED_INT:
                return 4;
            case GL_UNSIGNED_BYTE:
                return 1;
            default:
                assert(false);
            }
        }

        //VertexBufferElement(unsigned int type, unsigned int count, unsigned char normalized, unsigned int offset)
        //	: type(type), count(count), normalized(normalized), offset(offset) {
        //}
    };

    class VertexBufferLayout
    {
    private:
        std::vector<VertexBufferElement> m_Elements;
        unsigned int m_Stride;

    public:
        VertexBufferLayout(): m_Stride(0)
        {
        }

        ~VertexBufferLayout()
        = default;

        template <typename T>
        void Push(unsigned int count);


        [[nodiscard]] std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
        [[nodiscard]] unsigned int GetStride() const { return m_Stride; }
    };

    template <typename T>
    void VertexBufferLayout::Push(unsigned int count)
    {
        if constexpr (true)
            static_assert(false);
    }

    template <>
    inline void VertexBufferLayout::Push<float>(const unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count,GL_FALSE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
    }

    template <>
    inline void VertexBufferLayout::Push<unsigned>(const unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count,GL_FALSE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
    }

    template <>
    inline void VertexBufferLayout::Push<unsigned char>(const unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count,GL_TRUE});
        m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
    }
