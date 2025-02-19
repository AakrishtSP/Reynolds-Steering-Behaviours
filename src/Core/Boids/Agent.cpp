// src/Core/Boids/Agent.cpp
#include "Agent.h"

#include "Core/States/States.h"
#include "Core/Utilities/Utils.h"


Agent::Agent(): Boid("Agent")
{
}

Agent::Agent(const glm::vec2& spawnPosition) : Boid("Agent") {
    setPosition(spawnPosition);  // Set initial position

    // So basically, the observed spped is constat, but the velocity is different
    const glm::vec2 velocity = randomVelocity(static_cast<float>(1), 0);
    setVelocity(velocity);

}

// void Agent::update(std::vector<std::pair<glm::vec2, std::shared_ptr<Boid>>>)
// {
// }

void Agent::update(const std::vector<std::unique_ptr<Boid>>& boids, const States& states){
    for(auto& boid : boids){
        boid->findNeighbors(boids,states.getInfluenceRadius());
    }

    glm::vec2 position = getPosition();
    if(position.x < static_cast<float>(-680))
    {
        position.x = static_cast<float>(680);
    }
    if(position.x > static_cast<float>(680))
    {
        position.x = static_cast<float>(-680);
    }
    if(position.y < static_cast<float>(-380))
    {
        position.y = static_cast<float>(380);
    }
    if(position.y > static_cast<float>(380))
    {
        position.y = static_cast<float>(-380);
    }
    setPosition(position);
}



// glm::vec2 Agent::randomVelocity(const float requiredSpeed)
// {
//     static std::random_device rd;
//     static std::mt19937 gen(rd());
//     static std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * 3.1415);

//     const float angle = angle_dist(gen); // Random angle in [0, 2 pi]

//     const glm::vec2 velocity = glm::vec2(glm::cos(angle), glm::sin(angle)) * requiredSpeed;
//     return velocity;
// }
