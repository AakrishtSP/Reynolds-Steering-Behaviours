#include "pch.h"
#include "Utils.h"

glm::vec2 randomVelocity(const float requiredSpeed, int notConstant = 0)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> angle_dist(0.0f, 2.0f * 3.1415);

    const float angle = angle_dist(gen); // Random angle in [0, 2 pi]
    float returnedSpeed = requiredSpeed;
    if (notConstant)
    {
        //make requiredSpeed 0 to given value
        static std::uniform_real_distribution<float> speed_dist(0.0f, requiredSpeed);
        returnedSpeed = speed_dist(gen);
        
    }
    const glm::vec2 velocity = glm::vec2(glm::cos(angle), glm::sin(angle)) * returnedSpeed;
    return velocity;
}