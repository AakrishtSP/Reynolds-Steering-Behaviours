// src/Core/BoidsManager.h
#pragma once

#include "States/States.h"
#include "Boids/Boid.h"
#include "Renderer/Draw.h"

#include <vector>
#include <memory>

class BoidsManager {

public:
   BoidsManager();
   ~BoidsManager();

   void update();

   void addBoid(std::unique_ptr<Boid> boid);

    Draw& getDraw() { return m_Draw; }

   std::vector<std::unique_ptr<Boid>> &getBoids() { return m_boid; }

   //getstates
   States& getStates();

    [[nodiscard]] float getSize() const;
   void setSize(float size);
    void setBoids(std::vector<std::unique_ptr<Boid>> boids) { m_boid = std::move(boids); }

private:
   std::vector<std::unique_ptr<Boid>> m_boid;
   Draw m_Draw;
   States m_UpdateStates;
    float m_size;
};
