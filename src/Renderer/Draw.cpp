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

struct VerticesInfo
{
    float x;
    float y;
    float z;
};
struct BoidVertices
{
    VerticesInfo front_tip;
    VerticesInfo front_left;
    VerticesInfo front_right;
    VerticesInfo back_tip;
};

void Draw::init()
{
    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->Bind();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 0, true);
    m_IndexBuffer = std::make_unique<IndexBuffer>(nullptr, 0, true);
    m_Shader = std::make_unique<Shader>("assets/shaders/boid.glsl.vert", "assets/shaders/boid.glsl.frag");

    m_Projection = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
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

    auto* boidVertices = new BoidVertices[boids.size()];
    auto* indices = new unsigned int[boids.size() * 6];

    for (size_t i = 0; i < boids.size(); i++)
    {
        const glm::vec2 facingDir = glm::normalize(boids[i]->getFacingDirection());
        const glm::vec2 perp(-facingDir.y, facingDir.x);
        const float size = boids[i]->getSize();
        const glm::vec2 position = boids[i]->getPosition();

        // Define proportions
        const float bodyLength = size * 1.0f;
        const float wingWidth = size * 0.1f;
        const float tailLength = size * 0.25f;

        // A multiplier to exaggerate the wing spread for a caret wing look.
        constexpr float wingMultiplier = 1.5f; // Adjust as needed

        // Top tip remains at the front.
        boidVertices[i].front_tip = {
            position.x + facingDir.x * bodyLength,
            position.y + facingDir.y * bodyLength,
            0.0f
        };

        // Wing vertices: offset further along the perpendicular direction.
        boidVertices[i].front_left = {
            position.x - facingDir.x * wingWidth + perp.x * wingWidth * wingMultiplier,
            position.y - facingDir.y * wingWidth + perp.y * wingWidth * wingMultiplier,
            0.0f
        };

        boidVertices[i].front_right = {
            position.x - facingDir.x * wingWidth - perp.x * wingWidth * wingMultiplier,
            position.y - facingDir.y * wingWidth - perp.y * wingWidth * wingMultiplier,
            0.0f
        };

        // Back tip is centered directly behind the boid (no perpendicular offset).
        boidVertices[i].back_tip = {
            position.x + facingDir.x * tailLength,
            position.y + facingDir.y * tailLength,
            0.0f
        };

        // Indices for two triangles (upper: tip + wings, lower: wings + back tip)
        indices[6 * i + 0] = 4 * i + 0; // front tip
        indices[6 * i + 1] = 4 * i + 1; // front left
        indices[6 * i + 2] = 4 * i + 3; // front right

        indices[6 * i + 3] = 4 * i + 0; // front left
        indices[6 * i + 4] = 4 * i + 2; // front right
        indices[6 * i + 5] = 4 * i + 3; // back tip

    }
    m_VertexBuffer->SetData(boidVertices, boids.size() * sizeof(BoidVertices));

    m_IndexBuffer->EditData(indices, boids.size() * 6);

    m_View = glm::translate(glm::mat4(1.0f), m_translation);

    const glm::mat4 mvp = m_Projection * m_View;
    m_Shader->Bind();
    m_Shader->SetUniformMat4f("u_MVP", mvp);

    Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);

    delete[] indices;
    delete[] boidVertices;
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
