#pragma once

#include "Core/Boids/Boid.h"
#include<memory>


class States {
public:
	void updateBoidsDeafult(const std::vector<std::unique_ptr<Boid>>& boids);	
	void updateAccordingToNeighbours(const std::vector<std::unique_ptr<Boid>>& boids);
	void updateAccordingToThreats();
};

