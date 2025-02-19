// src/Application/Application.h
#pragma once
#include "IApplication.h"
#include "Core/BoidsManager.h"
#include <memory>
#include "glm/glm.hpp"



class Application final : public IApplication {
public:
    Application() = default;
    void init() override;
    void onUpdate(float deltaTime) override;
    void onImguiUpdate(float deltaTime) override;
    void shutdown() override;
private:
    std::unique_ptr<BoidsManager> m_BoidsManager;

    bool m_isSpawning{false};
    bool m_onCooldown{false};
    glm::vec2 m_translation{0,0};
    glm::vec2 m_spawnPosition{0,0};
    float m_radiusOfInfluence{70.0F};

    glm::vec4 m_obstacleBounds{ -1280 / 2, 720 / 2, 1280 / 2, - 720 / 2 };

    float m_influenceFactor{0.05F};
    float m_agentTerminalSpeed{3.0F};
    float m_randomnessFactor{0.5F};
    float m_size{30.0F};
};
