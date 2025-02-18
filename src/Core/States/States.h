#pragma once

#include "Core/Obstacle/Obstacle.h"
#include "Core/Boids/Boid.h"
#include<memory>



class Boid;

class States {

private:
	float m_influenceFactor = 0.05;
	float m_agentTerminalSpeed = 3.0f;
	float m_infludenceRadius = 70;
	float m_randomnessFactor = 0.5;
	int m_randomnessInterval = 30;

	//vObstacle obs({1.0f,1.0f,1.0f,1.0f});

public:
	void updateBoidsDeafult(const std::vector<std::unique_ptr<Boid>>& boids, const std::vector<std::unique_ptr<Obstacle>>& obstacles) const ;
	void updateAccordingToNeighbours(const std::vector<std::unique_ptr<Boid>>& boids) const;
	 void updateAccordingToBounds(const std::vector<std::unique_ptr<Obstacle>>& obstacles, const std::vector<std::unique_ptr<Boid>>& boids)const;
	void updateAccordingToThreats();


	[[nodiscard]] float getInfluenceFactor() const;
	[[nodiscard]] float getAgentTerminalSpeed() const;
	[[nodiscard]] float getInfluenceRadius() const;

	void setInfluenceFactor(float sIF);
	void setAgentTerminalSpeed(float aTS);
	void setInfluenceRadius(float iR);
	void setRandomnessFactor(float rF);
};

