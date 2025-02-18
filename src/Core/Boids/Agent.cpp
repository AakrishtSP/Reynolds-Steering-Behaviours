// src/Core/Boids/Agent.cpp
#include "Agent.h"
#include <random>

#include "Core/States/States.h"
#include "Core/Utilities/Utils.h"


Agent::Agent(): Boid("Agent")
{	
}

Agent::Agent(const glm::vec2& spawnPosition) : Boid("Agent") {
	setPosition(spawnPosition);  // Set initial position

    // So basically, the observed spped is constat, but the velocity is different
    glm::vec2 velocity = randomVelocity(3.0f);
    setVelocity(velocity);
	
}

// void Agent::update(std::vector<std::pair<glm::vec2, std::shared_ptr<Boid>>>)
// {
// }

void Agent::update(const std::vector<std::unique_ptr<Boid>>& boids){
    for(auto& boid : boids){
        boid->findNeighbors(boids);
    }

    glm::vec2 position = getPosition();
    if(position.x < -680)
        position.x = 680;
    if(position.x > 680)
        position.x = -680;
    if(position.y < -380)
        position.y = 380;
    if(position.y > 380)
        position.y = -380;
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
