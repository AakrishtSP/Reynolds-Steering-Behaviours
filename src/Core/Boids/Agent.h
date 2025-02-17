// src/Core/Boids/Agent.h
#pragma once

#include "Boid.h"

class Agent final : public Boid{
public:
    Agent();
    Agent(const glm::vec2& spawnPosition);
    ~Agent() override = default;


    // This is not a good place to keep it, will do for now
    glm::vec2 randomVelocity(float requiredSpeed);
    // void update(std::vector<std::pair<glm::vec2, std::shared_ptr<Boid>>>) override;
    void update(const std::vector<std::unique_ptr<Boid>>& boids) override;

};
