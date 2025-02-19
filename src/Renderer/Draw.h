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
    void draw(const std::vector<std::unique_ptr<Boid>>& boids, float size);
    static void clear();
    static void setClearColor(float r, float g, float b, float a);
    void setTranslation(const glm::vec2& translation);
    void drawCircle(const glm::vec2& position, float radius,  glm::vec3 color= {0.0F, 1.0f,0.0f} , int LineWidth=1) const;

    void drawObstacle(const std::vector<std::unique_ptr<Obstacle>>& obstacles, glm::vec3 color= {1.0f, 0.0f,0.0f} , int LineWidth=10) const;
    void drawObstacleOutline(glm::vec4 bounds, glm::vec3 color= {1.0f, 0.0f,0.0f} , int LineWidth=10) const;


private:
    void initUnitCircle();
    void initObstacle();


    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<IndexBuffer> m_IndexBuffer;
    std::unique_ptr<Shader> m_Shader;
    glm::mat4 m_View{};
    glm::mat4 m_Projection{};
    glm::vec3 m_translation{};

    std::unique_ptr<VertexArray> m_ObstacleVAO;
    std::unique_ptr<VertexBuffer> m_ObstacleVBO;

    std::unique_ptr<VertexArray> m_UnitCircleVAO;
    std::unique_ptr<VertexBuffer> m_UnitCircleVBO;
    std::unique_ptr<Shader> m_UnitCircleShader;
    int m_UnitCircleSegmentCount{};
};
