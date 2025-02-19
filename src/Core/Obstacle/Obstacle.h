#pragma once
#include "glm/glm.hpp"


class Obstacle {
public:
    explicit Obstacle(glm::vec4 bounds);

    glm::vec4 getBounds();

private:
    glm::vec4 m_bounds;
};
