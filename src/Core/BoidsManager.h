// src/Core/BoidsManager.h
#pragma once

#include "States/States.h"
#include "Boids/Boid.h"
#include "Renderer/Draw.h"

#include <vector>
#include <memory>
#include "Obstacle/Obstacle.h"

class BoidsManager {

public:
   BoidsManager();
   ~BoidsManager();

   void update();

   void addBoid(std::unique_ptr<Boid> boid);
   void addObstacle(std::unique_ptr<Obstacle> obstacle);

    Draw& getDraw() { return m_Draw; }

   std::vector<std::unique_ptr<Boid>> &getBoids() { return m_boid; }

   States& getStates();

    [[nodiscard]] float getSize() const;
   void setSize(float size);
    void setBoids(std::vector<std::unique_ptr<Boid>> boids) { m_boid = std::move(boids); }
    void setObstacles(std::vector<std::unique_ptr<Obstacle>> obs) { m_obstacles = std::move(obs); }

private:
   std::vector<std::unique_ptr<Boid>> m_boid;
   std::vector<std::unique_ptr<Obstacle>> m_obstacles;
   Draw m_Draw;
   States m_UpdateStates;
    float m_size;
};
