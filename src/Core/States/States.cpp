#include "States.h"

void States::updateWander(const std::vector<std::unique_ptr<Boid>>& boids)
{
    for (const auto& b : boids) // Iterate through each boid
    {
        b->getFacingDirection();
        glm::vec2 pos = b->getPosition();
        pos.x += 0.5;
        pos.y += 0.5;
        b->setPosition(pos);
    }
}
