// src/Application/Application.h
#pragma once
#include "IApplication.h"
#include "Core/BoidsManager.h"
#include <memory>


class Application final : public IApplication {
public:
    Application() = default;
    void init() override;
    void onUpdate(float deltaTime) override;
    void onImguiUpdate(float deltaTime) override;
    void shutdown() override;
private:
    std::unique_ptr<BoidsManager> m_BoidsManager;

    glm::vec2 m_spawnPosition{0,0};
};
