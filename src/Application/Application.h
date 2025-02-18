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
    float m_radiusOfInfluence{70};
};
