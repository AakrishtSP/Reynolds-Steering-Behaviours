// src/Core/Boids/Agent.h
#pragma once

#include "Boid.h"

class Agent final : public Boid{
public:
    Agent();
    Agent(const glm::vec2& spawnPosition);
    ~Agent() override = default;

    void update(std::vector<std::pair<glm::vec2, std::shared_ptr<Boid>>>) override;
    // void update(std::vector<std::shared_ptr<Boid>>) override;

};
