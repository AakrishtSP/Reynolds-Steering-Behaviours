#pragma once

#include "Core/Boids/Boid.h"
#include"glm/glm.hpp"
#include<memory>


class States {
public:
	void updateWander(const std::vector<std::unique_ptr<Boid>>& boids);
	
};

