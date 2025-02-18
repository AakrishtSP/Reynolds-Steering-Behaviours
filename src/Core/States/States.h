#pragma once

#include "Core/Boids/Boid.h"
#include<memory>

class Boid;

class States {

private:
	float m_influenceFactor = 0.05;
	float m_agentTerminalSpeed = 3.0f;
	float m_infludenceRadius = 70;

public:
	void updateBoidsDeafult(const std::vector<std::unique_ptr<Boid>>& boids) const;
	void updateAccordingToNeighbours(const std::vector<std::unique_ptr<Boid>>& boids) const;
	void updateAccordingToThreats();


	[[nodiscard]] float getInfluenceFactor() const;
	[[nodiscard]] float getAgentTerminalSpeed() const;
	[[nodiscard]] float getInfluenceRadius() const;

	void setInfluenceFactor(float sIF);
	void setAgentTerminalSpeed(float aTS);
	void setInfluenceRadius(float iR);
};

