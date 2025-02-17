// src/Core/Boids/Agent.cpp
#include "Agent.h"

Agent::Agent(): Boid("Agent")
{
	
}

Agent::Agent(const glm::vec2& spawnPosition) : Boid("Agent") {
	setPosition(spawnPosition);  // Set initial position
}

void Agent::update(std::vector<std::pair<glm::vec2, std::shared_ptr<Boid>>>)
{
}

// void Agent::update(std::vector<std::shared_ptr<Boid>>)
// {
// }
