#include "Boid.h"

void Boid::findNeighbors(const std::vector<std::unique_ptr<Boid>>& boids, const float influenceRadius)
{
    m_neighbors.clear();
    neighbor_info info{};
    // std::vector<std::unique_ptr<Boid>> neighbors;
    for (const auto& boid : boids)
    {
        if (boid.get() == this)
            continue;
        if (glm::distance(m_position, boid->getPosition()) < influenceRadius)
        {
            info.position = boid->getPosition();
            info.velocity = boid->getVelocity();
            m_neighbors.push_back(info);
        }
    }
}