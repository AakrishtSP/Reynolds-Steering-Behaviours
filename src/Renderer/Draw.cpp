// src/Renderer/Draw.cpp
#include "Draw.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Core/Boids/Boid.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/matrix_transform.hpp"


Draw::Draw()
= default;

Draw::~Draw()
= default;

void Draw::init()
{
    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->Bind();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 0, true);
    m_IndexBuffer = std::make_unique<IndexBuffer>(nullptr, 0, true);
    m_Shader = std::make_unique<Shader>("assets/shaders/boid.glsl.vert", "assets/shaders/boid.glsl.frag");

    m_Projection = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f);
    // m_View = glm::translate(glm::mat3(1.0f), glm::vec2(0.0f, 0.0f));
}

void Draw::shutdown()
{
    IndexBuffer::Unbind();
    VertexBuffer::Unbind();
    VertexArray::Unbind();
    Shader::Unbind();
}

void Draw::draw(const std::vector<std::unique_ptr<Boid>>& boids)
{
    m_VertexArray->Bind();
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
    m_Shader->Bind();

    const size_t size = 9 * boids.size();

    auto* vertices = new float[size];

    for (size_t i = 0; i < boids.size(); i++)
    {
        const glm::vec2 facingdir = boids[i]->getFacingDirection();
        const glm::vec2 perp(-facingdir.y, facingdir.x); // Perpendicular direction
        const glm::vec2 triangleSize = facingdir * static_cast<float>(boids[i]->getSize());
        const glm::vec2 wingOffset = perp * static_cast<float>(boids[i]->getSize()) * 0.2f;
        const glm::vec2 position = boids[i]->getPosition();

        vertices[9*i + 0] = position.x + triangleSize.x;
        vertices[9*i + 1] = position.y + triangleSize.y;
        vertices[9 * i + 2] = 0.0f;

        vertices[9*i + 3] = position.x - triangleSize.x + wingOffset.x;
        vertices[9*i + 4] = position.y - triangleSize.y + wingOffset.y;
        vertices[9 * i + 5] = 0.0f;


        vertices[9*i + 6] = position.x - triangleSize.x - wingOffset.x;
        vertices[9*i + 7] = position.y - triangleSize.y - wingOffset.y;
        vertices[9 * i + 8] = 0.0f;

        // std::cout << "Boid " << i << " at " << boids[i]->getPosition().x << ", " << boids[i]->getPosition().y << "Facing: "<<facingdir.x<<", "<<facingdir.y<<"\n";
    }
    m_VertexBuffer->SetData(vertices, size * sizeof(float), true);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

    auto* indices = new unsigned int[boids.size() * 3];

    for (size_t i = 0; i < boids.size(); i++)
    {
        indices[3 * i + 0] = 3 * i + 0;
        indices[3 * i + 1] = 3 * i + 1;
        indices[3 * i + 2] = 3 * i + 2;
    }

    m_IndexBuffer->EditData(indices, boids.size() * 3);

    m_View = glm::translate(glm::mat4(1.0f), m_translation);

    const glm::mat4 mvp = m_Projection * m_View;
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);

    Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

    delete[] indices;
    delete[] vertices;
}

void Draw::clear()
{
}

void Draw::setClearColor(float r, float g, float b, float a)
{
}

void Draw::setTranslation(const glm::vec2& translation)
{
    m_translation = glm::vec3( translation, 0.0f);
}
