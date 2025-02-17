// src/Core/BoidsManager.cpp
#include "BoidsManager.h"

#include "Renderer/Draw.h"
#include "Core/Boids/Agent.h"
#include "imgui.h"
#include "glm/glm.hpp"

BoidsManager::BoidsManager()
{
    m_Draw.init();
    m_translation = glm::vec2(0.0f, 0.0f);
}

BoidsManager::~BoidsManager()
= default;

void BoidsManager::update()
{
    //Do something here


    //yo vanda tala kolai abstract garda pani hunxa

    // Yo translation chai screen ko hai not of boids
    ImGui::Begin("Transform");
    ImGui::DragFloat2("Transform X", &m_translation.x, 1.0f);
    ImGui::End();

    m_Draw.setTranslation(m_translation);
    if (!m_boid.empty())
    {
        m_Draw.draw(m_boid);
        m_UpdateStates.updateWander(m_boid);
    }

    // for (auto& boid : m_boid)
    // {
    //     boid->update(m_boid);
    // }
}

void BoidsManager::addBoid(std::unique_ptr<Boid> boid)
{
    m_boid.push_back(std::move(boid));
}
