// src/Core/Boids/Boid.h
#pragma once

#include "glm/glm.hpp"
#include <utility>
#include <vector>
#include <memory>

struct neighbor_info {
    glm::vec2 position;
    glm::vec2 velocity;
};

class Boid {
public:
    explicit Boid(std::string  name="Boid") : m_name(std::move(name)), m_position(0,0), m_facingDirection(1,0), m_velocity(0,0), m_size(30), m_infuenceRadius(70) {};
    virtual ~Boid() = default;
    virtual void update(std::vector<std::pair<glm::vec2, std::shared_ptr<Boid>>>)=0;
    // virtual void update(std::vector<std::shared_ptr<Boid>>)=0;

    // Getters
    [[nodiscard]] const glm::vec2& getPosition() const { return m_position; }
    [[nodiscard]] const glm::vec2& getVelocity() const { return m_velocity; }
    [[nodiscard]] const glm::vec2& getAcceleration() const { return m_acceleration; }
    [[nodiscard]] const std::string& getName() const { return m_name; }
    [[nodiscard]] const std::vector<glm::vec2>& getStates() const { return m_states; }
    [[nodiscard]] float getSize() const { return m_size; }
    [[nodiscard]] float getInfuenceRadius() const { return m_infuenceRadius; }
    [[nodiscard]] glm::vec2 getFacingDirection() const { return m_facingDirection; }
    [[nodiscard]] const std::vector<neighbor_info>& getNeighbors() const { return m_neighbors; }

    //Setters
    void setPosition(const glm::vec2& position) { m_position = position; }
    void setVelocity(const glm::vec2& velocity) { m_velocity = velocity; }
    void setAcceleration(const glm::vec2& acceleration) { m_acceleration = acceleration; }
    void setSize(const float size) { m_size = size; }

    void setInfuenceRadius(const float infuenceRadius) { m_infuenceRadius = infuenceRadius; }
    void setFacingDirection(glm::vec2& facing) { m_facingDirection = facing; }
  
    void findNeighbors(const std::vector<std::unique_ptr<Boid>>& boids);

    // std::vector<std::unique_ptr<Boid>> getNeighbours();


private:
        std::string m_name{};
        glm::vec2 m_position;
        glm::vec2 m_facingDirection;
        glm::vec2 m_velocity;
        glm::vec2 m_acceleration{};
        std::vector<glm::vec2> m_states;
        // std::vector<std::shared_ptr<Boid>> m_neighbors;
        std::vector<neighbor_info> m_neighbors;
        float m_size;
        float m_infuenceRadius;
};
