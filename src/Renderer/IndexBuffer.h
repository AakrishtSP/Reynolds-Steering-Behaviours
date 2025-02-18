// src/Renderer/IndexBuffer.h
#pragma once
class IndexBuffer
{
public:
    IndexBuffer(const unsigned int* data, unsigned int count, bool isDynamic = false);
    explicit IndexBuffer(unsigned int count);
    ~IndexBuffer();

    void EditData(const unsigned int* data, unsigned int count, unsigned int offset = 0, bool resize = false);
    //void EditData(void* data, unsigned int count, unsigned int offset = 0);

    void GenerateQuadDefault(unsigned int count, unsigned int offsetQuad = 0);

    void Bind() const;
    static void Unbind();

    [[nodiscard]] unsigned int GetCount() const { return m_Count; }
private:
    unsigned int m_RendererID{};
    unsigned int m_Count;
    unsigned int m_BufferSize;
    bool m_bIsDynamic;
    static unsigned int s_CurrentlyBound;
};