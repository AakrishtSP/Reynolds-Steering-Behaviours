#include "States.h"
#include "Core/Utilities/Utils.h"


void States::determineCluster(const std::vector<std::unique_ptr<Boid>>& boids) const
{
    const int clusterRadius = m_infludenceRadius;

    // Reset in-cluster flag for all boids before checking clusters
    for (const auto& boid : boids)
    {
        boid->resetInCluster();
    }

    // Determine clusters and set colors
    for (size_t i = 0; i < boids.size(); i++)
    {
        for (size_t j = 0; j < boids.size(); j++)
        {
            if (i == j) continue;

            // Check if in influence radius
            if (glm::distance(boids.at(i)->getPosition(), boids.at(j)->getPosition()) < clusterRadius)
            {
                boids.at(j)->setClusterColor(boids.at(i)->getClusterColor());
                boids.at(j)->setInCluster();
            }
        }
    }

    // Reset color for boids that are NOT in a cluster
    for (const auto& boid : boids)
    {
        if (!boid->isInCluster())
        {
            boid->setClusterColor(boid->getIndivisualColor());
        }
    }
}



void States::updateBoidsDeafult(const std::vector<std::unique_ptr<Boid>>& boids, const std::vector<std::unique_ptr<Obstacle>>& obstacles) const
{
    determineCluster(boids);

    updateAccordingToBounds(obstacles,boids);
    // Can put all the checks here
    updateAccordingToNeighbours(boids);



    for (const auto& b : boids) // Iterate through each boid
    {
        const glm::vec2 acc = b->getAcceleration();
        glm::vec2 vel = b->getVelocity();


        vel += acc;
        b->setChangeInterval(b->getChangeInterval() + 1);
        if (b->getChangeInterval() > m_randomnessInterval)
        {
            b->setChangeInterval(0);
            b->setRandomVelocity(randomVelocity(m_randomnessFactor, 1));
        }
        // glm::vec2 temp =randomVelocity(m_randomnessFactor,1);

        if (glm::distance(b->getRandomVelocity(),{0,0}) < (m_randomnessFactor/4))
        {
            b->setRandomVelocity({0,0});
        }
        vel += b->getRandomVelocity(); // Random velocity to add some randomness
        b->setVelocity(vel);
        b->setVelocity(glm::normalize(b->getVelocity()) * m_agentTerminalSpeed);
        b->setFacingDirection(vel);
        b->setPosition(b->getPosition() + b->getVelocity());
    }
}


void States::updateAccordingToBounds(const std::vector<std::unique_ptr<Obstacle>>& obstacles, const std::vector<std::unique_ptr<Boid>>& boids) const
{
    const float avoidanceStrength = 0.5f; // Smaller values make smoother turns
    const float avoidanceDistance = 100;

    for (const auto& boid : boids)
    {
        glm::vec2 pos = boid->getPosition();
        glm::vec2 vel = boid->getVelocity();
        glm::vec2 correction = { 0, 0 };

        for (const auto& obs : obstacles)
        {
            glm::vec4 bounds = obs->getBounds(); // {left, top, right, bottom}

            // Compute distance from boundary
            float leftDist = abs(bounds.x - pos.x);
            float rightDist = abs(bounds.z - pos.x);
            float topDist = abs(bounds.y - pos.y);
            float bottomDist = abs(pos.y - bounds.w);

            // Apply scaled avoidance force
            if (leftDist < avoidanceDistance)
                correction.x += (1.0f - leftDist / avoidanceDistance) * m_agentTerminalSpeed;
            else if (rightDist < avoidanceDistance)
                correction.x -= (1.0f - rightDist / avoidanceDistance) * m_agentTerminalSpeed;

            if (topDist < avoidanceDistance)
                correction.y -= (1.0f - topDist / avoidanceDistance) * m_agentTerminalSpeed;
            else if (bottomDist < avoidanceDistance)
                correction.y += (1.0f - bottomDist / avoidanceDistance) * m_agentTerminalSpeed;
        }

        // Apply gradual correction
        if (glm::length(correction) > 0.0f)
        {
            correction *= avoidanceStrength; // Scale down the correction for smoothness
            vel = glm::normalize(vel + correction) * m_agentTerminalSpeed;
            boid->setVelocity(vel);
        }
    }
}


// Now for each boid i will have it's neighbours
// i.e for b in boids, i will have 
// b->getNeighbours() 
// which will be of type std::vector<std::unique_ptr<Boid>>


void States::updateAccordingToNeighbours(const std::vector<std::unique_ptr<Boid>>& boids) const
{
    for (const auto& b : boids)
    {   
        std::vector<neighbor_info> allNeighbours = b->getNeighbors();
        glm::vec2 res_vel = {0, 0};
        float totalWeight = 0.0f;

        for (const auto& [position, velocity] : allNeighbours)
        {
            const float distance = glm::length(position - b->getPosition());
            if (distance == 0.0f) continue;

            // Inverse distance weighting -> Farther the neighbour, lesser the influence
            const float weight = 1.0f / distance;
            res_vel += weight * velocity;
            totalWeight += weight;
        }

        if (totalWeight > 0.0f)
        {
            res_vel /= totalWeight; // This will just normalize the resultant velocity
            res_vel = glm::normalize(res_vel) * m_agentTerminalSpeed;
            // Preserve speed of 2.0f , I think it should not be hard coded
        }
        else
        {
            res_vel = glm::normalize(b->getVelocity()) * m_agentTerminalSpeed; // Keep current velocity if no neighbors
        }
        res_vel -= b->getVelocity();
        res_vel *= m_influenceFactor;


        /*res_vel = b->getVelocity() + res_vel;
        b->setVelocity(res_vel);*/

        b->setAcceleration(res_vel);

        //b->setAcceleration()
        // b->setVelocity(res_vel);
        
    }
}

void States::updateAccordingToThreats()
{
}

float States::getInfluenceFactor() const
{
    return m_influenceFactor;
}

float States::getAgentTerminalSpeed() const
{
    return m_agentTerminalSpeed;
}

float States::getInfluenceRadius() const
{
    return m_infludenceRadius;
}


void States::setInfluenceFactor(const float sIF) { m_influenceFactor = sIF; }

void States::setAgentTerminalSpeed(const float aTS) { m_agentTerminalSpeed = aTS; }

void States::setInfluenceRadius(const float iR) { m_infludenceRadius = iR; }

void States::setRandomnessFactor(const float rF)
{
    m_randomnessFactor = rF;
}
