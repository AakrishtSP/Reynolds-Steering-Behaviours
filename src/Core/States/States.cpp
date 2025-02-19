#include "States.h"
#include "Core/Utilities/Utils.h"


#include <queue>
#include <unordered_map>

// Store all unique clusters and their assigned colors so we can access their vallues accross multiple frames
static std::unordered_map<size_t, glm::vec3> clusterColors;


void States::determineCluster(const std::vector<std::unique_ptr<Boid>>& boids) const
{
    const float clusterRadius = m_infludenceRadius;
    const size_t nBoids = boids.size();

    std::vector<bool> visited(nBoids, false);

    // Reset cluster colors before checking new clusters -> We assume that none of the boids are in cluster
    for (const auto& boid : boids) {
        boid->resetInCluster();
    }


    for (size_t i = 0; i < nBoids; ++i) {
        if (visited[i]) continue; // Skip if already processed

        std::vector<size_t> clusterIndices;  // -> Stores the indicies of boids in current cluster
        std::queue<size_t> queue;
        queue.push(i);
        visited.at(i) = true;

        // BFS to find all boids connected to boid i
        while (!queue.empty()) {
            size_t current = queue.front();
            queue.pop();
            clusterIndices.push_back(current);


            /* Basically:
                Pick a boid that hasnt been assigned a cluster yet
                Find all boids near it (within a certain radius)
                    Repeat for each nearby boid until we get a full group, Thus the BSF kind of like flood fill garne bela ma gare jastai
                Store this group as a cluster and move to the next unvisited boid.
            */
            for (size_t j = 0; j < nBoids; ++j) {
                // If a boid belong to the cluster, add it's index to the queue of indices for current initlized cluster and mark it visited
                if (!visited[j] && glm::distance(boids[current]->getPosition(), boids[j]->getPosition()) < clusterRadius) {
                    visited[j] = true;
                    queue.push(j);
                }
            }
        }

        // Yeti vayasi we basically just calculated a unique cluster by picking a boid that has not been visited yet i.e. was not a part of a cluster
        // Now need to assign a color to the cluster which is again TRICKY


        // Find a unique identifier for the cluster basically can pick any boid in this cluster which we just calculated
        size_t clusterID = clusterIndices[0];

        // If this cluster already existed in the previous frame, keep its color Else assign a different random color
        glm::vec3 clusterColor;
        if (clusterColors.find(clusterID) != clusterColors.end()) {
            clusterColor = clusterColors[clusterID];
        }
        else {
            // Generate a new color for the cluster
            clusterColor = glm::vec3(getRandom(0, 1), getRandom(0, 1), getRandom(0, 1));
            clusterColors[clusterID] = clusterColor; // Store it
        }

        // Assign this color to all the boids in the cluster we just calculated
        for (auto idx : clusterIndices) {
            boids[idx]->setClusterColor(clusterColor);
            boids[idx]->setInCluster();
        }
    }

    // Remove stale cluster colors (clusters that no longer exist)
    for (auto it = clusterColors.begin(); it != clusterColors.end();) {
        if (!visited[it->first]) {
            it = clusterColors.erase(it); // Remove stale entry
        }
        else {
            ++it;
        }
    }

    // Reset color for boids not in a cluster
    for (const auto& boid : boids) {
        if (!boid->isInCluster()) {
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
    const float avoidanceDistanceExterior = avoidanceDistance/2;

    for (const auto& boid : boids)
    {
        glm::vec2 pos = boid->getPosition();
        glm::vec2 vel = boid->getVelocity();
        glm::vec2 correction = { 0, 0 };

        for (const auto& obs : obstacles)
        {
            const glm::vec4 bounds = obs->getBounds(); // {left, top, right, bottom}

            // Check if boid is inside the obstracle, 
            bool insideX = pos.x >= bounds.x && pos.x <= bounds.z;
            bool insideY = pos.y >= bounds.w && pos.y <= bounds.y;
            bool inside = insideX && insideY;


            // Compute distance from boundary
            const float leftDist = abs(bounds.x - pos.x);
            const float rightDist = abs(bounds.z - pos.x);
            const float topDist = abs(bounds.y - pos.y);
            const float bottomDist = abs(pos.y - bounds.w);

            // If inside, then paila j theo tai
            if(inside){

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
            
            else {
                if (leftDist < avoidanceDistanceExterior)
                    correction.x -= (1.0f - leftDist / avoidanceDistanceExterior) * m_agentTerminalSpeed;
                else if (rightDist < avoidanceDistanceExterior)
                    correction.x += (1.0f - rightDist / avoidanceDistanceExterior) * m_agentTerminalSpeed;

                if (topDist < avoidanceDistanceExterior)
                    correction.y += (1.0f - topDist / avoidanceDistanceExterior) * m_agentTerminalSpeed;
                else if (bottomDist < avoidanceDistanceExterior)
                    correction.y -= (1.0f - bottomDist / avoidanceDistanceExterior) * m_agentTerminalSpeed;
            }
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
