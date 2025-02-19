#include "Obstacle.h"


Obstacle::Obstacle(glm::vec4 bounds): m_bounds(bounds) {}


glm::vec4 Obstacle::getBounds() { return m_bounds; }

