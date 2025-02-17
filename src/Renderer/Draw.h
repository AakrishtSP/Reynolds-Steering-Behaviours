// src/Renderer/Draw.h
#pragma once
#include "Core/Boids/Boid.h"
#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "glm/glm.hpp"

class Draw {
public:
    Draw();
    ~Draw();
    void init();
    static void shutdown();
    void draw(const std::vector<std::unique_ptr<Boid>>& boids);
    static void clear();
    static void setClearColor(float r, float g, float b, float a);
    void setTranslation(const glm::vec2& translation);
    void drawCircle(const glm::vec2& position, float radius) const;

private:
    void initUnitCircle();


    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    glm::mat4 m_View{};
    glm::mat4 m_Projection{};
    glm::vec3 m_translation{};

    std::unique_ptr<VertexArray> m_UnitCircleVAO;
    std::unique_ptr<VertexBuffer> m_UnitCircleVBO;
    std::unique_ptr<Shader> m_UnitCircleShader;
    int m_UnitCircleSegmentCount{};
};
