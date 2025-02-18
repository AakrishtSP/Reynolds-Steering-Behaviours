#pragma once

#include "Core/Boids/Boid.h"
#include<memory>


class States {

private:
	float influenceFactor = 0.05;
	float agentTerminalSpeed = 3.0f;

public:
	void updateBoidsDeafult(const std::vector<std::unique_ptr<Boid>>& boids);	
	void updateAccordingToNeighbours(const std::vector<std::unique_ptr<Boid>>& boids);
	void updateAccordingToThreats();

	void setInfluenceFactor(float sIF);
	void setAgentTerminalSpeed(float aTS);


};

