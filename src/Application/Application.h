// src/Application/Application.h
#pragma once
#include "IApplication.h"
#include <memory>
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

class Application final : public IApplication {
public:
    Application() = default;
    void init() override;
    void onUpdate(float deltaTime) override;
    void onImguiUpdate(float deltaTime) override;
    void shutdown() override;
private:
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
};
