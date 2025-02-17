#include "Boid.h"

void Boid::findNeighbors(const std::vector<std::unique_ptr<Boid>>& boids)
{
    m_neighbors.clear();
    // std::vector<std::unique_ptr<Boid>> neighbors;
    for (const auto& boid : boids)
    {
        if (boid.get() == this)
            continue;

        if (glm::distance(m_position, boid->getPosition()) < m_infuenceRadius)
        {
            m_neighbors.push_back(boid.get());
        }
    }
}