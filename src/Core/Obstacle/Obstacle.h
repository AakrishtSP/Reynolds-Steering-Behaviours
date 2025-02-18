#pragma once
#include "glm/glm.hpp"
#include <utility>
#include <vector>
#include <memory>

#include "Core/States/States.h"
#include "Core/Utilities/Utils.h"

class Obstacle {
public:
    Obstacle(glm::vec4 bounds);

    // bool IsColliding(const Boid& boid) const; // Check if a boid is inside the obstacle range

    glm::vec4 getBounds();

private:
    glm::vec4 m_bounds;
};
