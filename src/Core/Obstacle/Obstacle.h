#pragma once
#include <vector>
#include "glm/glm.hpp"


class Obstacle {
public:
    Obstacle(glm::vec4 bounds);

    glm::vec4 getBounds();

private:
    glm::vec4 m_bounds;
};
