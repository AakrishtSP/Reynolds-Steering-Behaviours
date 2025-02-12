// src/Application/Application.cpp
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexBufferLayout.h"
#include "imgui.h"

void Application::init()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
        0, 1, 2
    };

    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->Bind();
    m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices));
    m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

    m_Shader = std::make_unique<Shader>("assets/shaders/basic.glsl.vert", "assets/shaders/basic.glsl.frag");

}

void Application::onUpdate(float deltaTime)
{
    Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
}

void Application::onImguiUpdate(float deltaTime)
{
    ImGui::Text("This is a test window.");
}

void Application::shutdown()
{
}
