#pragma once

#include "Core/Obstacle/Obstacle.h"
#include "Core/Boids/Boid.h"
#include<memory>



class Boid;

class States {

private:
    float m_influenceFactor = 0.08F;
    float m_agentTerminalSpeed = 3.0F;
    float m_infludenceRadius = 80.0F;
    float m_randomnessFactor = 0.3F;
    int m_randomnessInterval = 30;

public:
    void updateBoidsDeafult(const std::vector<std::unique_ptr<Boid>>& boids, const std::vector<std::unique_ptr<Obstacle>>& obstacles) const ;
    void updateAccordingToNeighbours(const std::vector<std::unique_ptr<Boid>>& boids) const;
     void updateAccordingToBounds(const std::vector<std::unique_ptr<Obstacle>>& obstacles, const std::vector<std::unique_ptr<Boid>>& boids)const;
    void updateAccordingToThreats();

    void determineCluster(const std::vector<std::unique_ptr<Boid>>& boids) const;



    [[nodiscard]] float getInfluenceFactor() const;
    [[nodiscard]] float getAgentTerminalSpeed() const;
    [[nodiscard]] float getInfluenceRadius() const;

    void setInfluenceFactor(float sIF);
    void setAgentTerminalSpeed(float aTS);
    void setInfluenceRadius(float iR);
    void setRandomnessFactor(float rF);
};

