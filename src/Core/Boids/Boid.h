// src/Core/Boids/Boid.h
#pragma once

#include "glm/glm.hpp"
#include <utility>
#include <vector>
#include <memory>

#include "Core/States/States.h"
#include "Core/Utilities/Utils.h"

class States;

struct neighbor_info {
    glm::vec2 position;
    glm::vec2 velocity;
};

class Boid {
public:
    explicit Boid(std::string  name="Boid") :m_clusterColor(0,0,0),m_inCluster(false),m_indivisualColor(0,0,0), m_name(std::move(name)), m_position(0, 0), m_facingDirection(1, 0), m_velocity(0, 0), m_randomVelocity({0,0}), m_changeInterval(static_cast<int>(getRandom(0, 30))) {};
    virtual ~Boid() = default;
    virtual void update(const std::vector<std::unique_ptr<Boid>>& boids, const States& states)=0;

    // Getters
    [[nodiscard]] const glm::vec2& getPosition() const { return m_position; }
    [[nodiscard]] const glm::vec2& getVelocity() const { return m_velocity; }
    [[nodiscard]] const glm::vec2& getAcceleration() const { return m_acceleration; }
    [[nodiscard]] const glm::vec2& getRandomVelocity() const { return m_randomVelocity; }
    [[nodiscard]] const std::string& getName() const { return m_name; }
    [[nodiscard]] glm::vec2 getFacingDirection() const { return m_facingDirection; }
    [[nodiscard]] const std::vector<neighbor_info>& getNeighbors() const { return m_neighbors; }
    [[nodiscard]] int getChangeInterval() const { return m_changeInterval; }
    [[nodiscard]] const glm::vec3& getClusterColor() {return m_clusterColor;}
    [[nodiscard]] const glm::vec3& getIndivisualColor() { return m_indivisualColor;}
    [[nodiscard]] const bool isInCluster() { return m_inCluster; }

    //Setters
    void setPosition(const glm::vec2& position) { m_position = position; }
    void setVelocity(const glm::vec2& velocity) { m_velocity = velocity; }
    void setAcceleration(const glm::vec2& acceleration) { m_acceleration = acceleration; }
    void setChangeInterval(const int changeInterval) { m_changeInterval = changeInterval; }
    void setFacingDirection(const glm::vec2& facing) { m_facingDirection = facing; }
    void setRandomVelocity(const glm::vec2& randomVelocity) { m_randomVelocity = randomVelocity; }
    void setClusterColor(const glm::vec3& clusterColor) { m_clusterColor = clusterColor; }
    void setInCluster() { m_inCluster = true; }
    void resetInCluster() { m_inCluster = false; }
    void setIndivisualColor(const glm::vec3& indivisualColor) { m_indivisualColor = indivisualColor; }

    void findNeighbors(const std::vector<std::unique_ptr<Boid>>& boids,float influenceRadius);

private:
        std::string m_name{};
        glm::vec2 m_position;
        glm::vec2 m_facingDirection;
        glm::vec2 m_velocity;
        glm::vec2 m_acceleration{};
        glm::vec2 m_randomVelocity;
        std::vector<neighbor_info> m_neighbors;
        int m_changeInterval;
        glm::vec3 m_clusterColor;
        bool m_inCluster;
        glm::vec3 m_indivisualColor;

};
