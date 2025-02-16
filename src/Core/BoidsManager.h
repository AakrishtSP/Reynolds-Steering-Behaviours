// src/Core/BoidsManager.h
#pragma once

#include "States/States.h"
#include "Boids/Boid.h"

#include <vector>
#include <memory>

class BoidsManager {

public:
   BoidsManager();
   ~BoidsManager();

   void update();

   void addBoid(std::unique_ptr<Boid> boid);

 private:
   std::vector<std::unique_ptr<Boid>> m_boid;
};
