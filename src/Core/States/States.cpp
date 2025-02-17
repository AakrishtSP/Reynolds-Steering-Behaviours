#include "States.h"

void States::updateWander(const std::vector<std::unique_ptr<Boid>>& boids)
{
    for (const auto& b : boids) // Iterate through each boid
    {
        glm::vec2 vel = b->getVelocity();
        b->setFacingDirection(vel);
        b->setPosition(b->getPosition() + vel);
    }
}
