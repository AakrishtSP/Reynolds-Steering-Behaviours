// src/Renderer/Draw.cpp
#include "Draw.h"

#include <cmath>
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
    glm::vec3 position;
    glm::vec3 color;
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
    Renderer::SetClearColor(0.117f, 0.137f, 0.156f, 1.0f);
    m_VertexArray = std::make_unique<VertexArray>();
    m_VertexArray->Bind();
    m_VertexBuffer = std::make_unique<VertexBuffer>(nullptr, 0, true);
    m_IndexBuffer = std::make_unique<IndexBuffer>(nullptr, 0, true);
    m_Shader = std::make_unique<Shader>("assets/shaders/boid.glsl.vert", "assets/shaders/boid.glsl.frag");

    m_Projection = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(3);
    m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

    initUnitCircle();
    initObstacle();
    // glDisable(GL_CULL_FACE);
}

void Draw::shutdown()
{
    IndexBuffer::Unbind();
    VertexBuffer::Unbind();
    VertexArray::Unbind();
    Shader::Unbind();
}

void Draw::draw(const std::vector<std::unique_ptr<Boid>>& boids, const float size)
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
        const glm::vec2 position = boids[i]->getPosition();

        // Define proportions
        const float bodyLength = size * 1.0f;
        const float wingWidth = size * 0.2f;
        const float tailLength = size * 0.25f;

        // A multiplier to exaggerate the wing spread for a caret wing look.
        constexpr float wingMultiplier = 1.5f; // Adjust as needed

        const glm::vec3 color = boids[i]->getClusterColor();

        // Top tip remains at the front.
        boidVertices[i].front_tip.position = {

                position.x + facingDir.x * bodyLength,
                position.y + facingDir.y * bodyLength,
                0.0f
        };
        boidVertices[i].front_tip.color = color;

        // Wing vertices: offset further along the perpendicular direction.
        boidVertices[i].front_left.position = {
            position.x - facingDir.x * wingWidth + perp.x * wingWidth * wingMultiplier,
            position.y - facingDir.y * wingWidth + perp.y * wingWidth * wingMultiplier,
            0.0f
        };
        boidVertices[i].front_left.color = color;

        boidVertices[i].front_right.position = {
            position.x - facingDir.x * wingWidth - perp.x * wingWidth * wingMultiplier,
            position.y - facingDir.y * wingWidth - perp.y * wingWidth * wingMultiplier,
            0.0f
        };
        boidVertices[i].front_right.color = color;

        // Back tip is centered directly behind the boid (no perpendicular offset).
        boidVertices[i].back_tip.position = {
            position.x + facingDir.x * tailLength,
            position.y + facingDir.y * tailLength,
            0.0f
        };
        boidVertices[i].back_tip.color = color;

        // Indices for two triangles (upper: tip + wings, lower: wings + back tip)
        indices[6 * i + 0] = 4 * i + 0; // front tip
        indices[6 * i + 1] = 4 * i + 1; // front left
        indices[6 * i + 2] = 4 * i + 3; // back tip

        indices[6 * i + 3] = 4 * i + 0; // front tip
        indices[6 * i + 4] = 4 * i + 2; // front right
        indices[6 * i + 5] = 4 * i + 3; // back tip

    }
    m_VertexBuffer->SetData(boidVertices, boids.size() * sizeof(BoidVertices));

    m_IndexBuffer->EditData(indices, boids.size() * 6, 0 , true);

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

void Draw::setClearColor(const float r, const float g, const float b, const float a)
{
    Renderer::SetClearColor(r, g, b, a);
}

void Draw::setTranslation(const glm::vec2& translation)
{
    m_translation = glm::vec3( translation, 0.0f);
}

void Draw::drawCircle(const glm::vec2& position, const float radius, const glm::vec3 color , const int LineWidth) const
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

    const glm::mat4 mvp = m_Projection * m_View * model;

    m_UnitCircleShader -> Bind();
    m_UnitCircleShader -> SetUniformMat4f("u_MVP", mvp);
    m_UnitCircleShader -> SetUniform4f("u_Color", color.r, color.g, color.b, 1.0f);

    m_UnitCircleVAO -> Bind();
    glLineWidth(static_cast<float>(LineWidth));
    glDrawArrays(GL_LINE_LOOP, 0, m_UnitCircleSegmentCount);

}

void Draw::drawObstacle(const std::vector<std::unique_ptr<Obstacle>>& obstacles, const glm::vec3 color , const int LineWidth) const
{
    m_ObstacleVAO->Bind();
    m_ObstacleVBO->Bind();
    m_UnitCircleShader->Bind();

    auto* vertices = new glm::vec3[obstacles.size() * 8];
    for (size_t i = 0; i < obstacles.size(); i++)
    {
        const glm::vec4 bounds = obstacles[i]->getBounds();
        vertices[8 * i + 0] = { bounds.x, bounds.y, 0.0f };
        vertices[8 * i + 1] = { bounds.z, bounds.y, 0.0f };
        vertices[8 * i + 2] = { bounds.z, bounds.y, 0.0f };
        vertices[8 * i + 3] = { bounds.z, bounds.w, 0.0f };
        vertices[8 * i + 4] = { bounds.z, bounds.w, 0.0f };
        vertices[8 * i + 5] = { bounds.x, bounds.w, 0.0f };
        vertices[8 * i + 6] = { bounds.x, bounds.w, 0.0f };
        vertices[8 * i + 7] = { bounds.x, bounds.y, 0.0f };

    }

    const glm::mat4 mvp = m_Projection * m_View;
    m_UnitCircleShader -> SetUniformMat4f("u_MVP", mvp);
    m_UnitCircleShader -> SetUniform4f("u_Color", color.r, color.g, color.b, 1.0f);

    m_ObstacleVBO->SetData(vertices, obstacles.size() * 8 * sizeof(glm::vec3));
    glLineWidth(static_cast<float>(LineWidth));
    glDrawArrays(GL_LINES, 0, static_cast<int>(obstacles.size()) * 8);

    delete[] vertices;
}

void Draw::initUnitCircle()
{
    constexpr int segments = 64;
    std::vector<float> unitCircleVertices;
    unitCircleVertices.reserve(segments * 3);
    for (int i = 0; i < segments; ++i)
    {
        const float angle = 2.0f * glm::pi<float>() * static_cast<float>(i) / segments;
        unitCircleVertices.push_back(std::cos(angle));  // x
        unitCircleVertices.push_back(std::sin(angle));   // y
        unitCircleVertices.push_back(0.0f);           // z
    }
    m_UnitCircleShader = std::make_unique<Shader>("assets/shaders/circle.glsl.vert", "assets/shaders/circle.glsl.frag");
    m_UnitCircleVAO = std::make_unique<VertexArray>();
    m_UnitCircleVBO = std::make_unique<VertexBuffer>(unitCircleVertices.data(), unitCircleVertices.size() * sizeof(float), false);
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_UnitCircleVAO->AddBuffer(*m_UnitCircleVBO, layout);
    m_UnitCircleSegmentCount = segments;
}

void Draw::initObstacle()
{
    m_ObstacleVAO = std::make_unique<VertexArray>();
    m_ObstacleVBO = std::make_unique<VertexBuffer>(nullptr, 0, true);
    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_ObstacleVAO->AddBuffer(*m_ObstacleVBO, layout);

}
