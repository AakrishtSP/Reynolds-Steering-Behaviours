// src/Core/Boids/Boid.h
#pragma once

#include "glm/glm.hpp"
#include <utility>
#include <vector>
#include <memory>

class Boid {
public:
    explicit Boid(std::string  name="Boid") : m_name(std::move(name)) {};
    virtual ~Boid() = default;
    virtual void update(std::vector<std::pair<glm::vec2, std::shared_ptr<Boid>>>)=0;
private:
        size_t m_size{};
        glm::vec2 m_position{};
        glm::vec2 m_velocity{};
        glm::vec2 m_acceleration{};
        std::vector<glm::vec2> m_states;
        std::string m_name{};
};
