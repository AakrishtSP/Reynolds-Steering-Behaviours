// src/Renderer/Draw.h
#pragma once
#include "Core/Boids/Boid.h"
#include <vector>
#include <memory>

class Draw {
public:
    static void draw(std::vector<std::shared_ptr<Boid>> boids);
    static void clear();
    static void setClearColor(float r, float g, float b, float a);
};
