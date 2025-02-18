#include "pch.h"
#include "Utils.h"

glm::vec2 randomVelocity(const float requiredSpeed)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * 3.1415);

    const float angle = angle_dist(gen); // Random angle in [0, 2 pi]

    const glm::vec2 velocity = glm::vec2(glm::cos(angle), glm::sin(angle)) * requiredSpeed;
    return velocity;
}