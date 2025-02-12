// src/Renderer/VertexBuffer.h
#pragma once

class VertexBuffer
{
public:
    VertexBuffer(const void* data, unsigned int size, bool isDynamic=false);
    explicit VertexBuffer(unsigned int size);
    ~VertexBuffer();

    void SetData(const void* data, unsigned int size, bool resize=false);
    void EditData(const void* data, unsigned int size, unsigned int offset=0) const;

    void Bind() const;
    static void Unbind();
private:
    unsigned int m_RendererID{};
    bool m_bIsDynamic;
    unsigned int m_Size;
    static unsigned int s_CurrentlyBound;
};