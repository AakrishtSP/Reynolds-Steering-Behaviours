// src/Core/BoidsManager.cpp
#include "BoidsManager.h"
#include "Utilities/Utils.h"
#include "Renderer/Draw.h"

BoidsManager::BoidsManager(): m_size(30)
{
    m_Draw.init();
    glm::vec4 mainBounds = { -1280 / 2, 720 / 2, 1280 / 2, - 720 / 2 };
    std::unique_ptr<Obstacle> mainObs = std::make_unique<Obstacle>(mainBounds);
    addObstacle(std::move(mainObs));

    // Test bounds
    glm::vec4 test_bound = { -100,100,100,-100 };
    std::unique_ptr<Obstacle> testObs = std::make_unique<Obstacle>(test_bound);
    addObstacle(std::move(testObs));
    
}

BoidsManager::~BoidsManager()
= default;

void BoidsManager::update()
{
    //Do something here


    //yo vanda tala kolai abstract garda pani hunxa

    m_Draw.drawObstacle(m_obstacles);
    if (!m_boid.empty())
    {
        m_Draw.draw(m_boid, m_size);
        m_UpdateStates.updateBoidsDeafult(m_boid,m_obstacles);
    }

    for (const auto& boid : m_boid)
    {
        boid->update(m_boid, m_UpdateStates);
    }
}

void BoidsManager::addBoid(std::unique_ptr<Boid> boid)
{
    const glm::vec3 initial_color = { getRandom(0, 1)  , getRandom(0, 1), getRandom(0, 1) };
    boid->setIndivisualColor(initial_color);
    boid->setClusterColor(initial_color);
    boid->resetInCluster();
    m_boid.push_back(std::move(boid));
}


void BoidsManager::defineObstracle(glm::vec4 bounds)
{
    glm::vec4 test_bound = bounds;
    std::unique_ptr<Obstacle> testObs = std::make_unique<Obstacle>(test_bound);
    addObstacle(std::move(testObs));

}


void BoidsManager::addObstacle(std::unique_ptr<Obstacle> obs)
{
    m_obstacles.push_back(std::move(obs));
}


States& BoidsManager::getStates()
{
    return this->m_UpdateStates;
}

float BoidsManager::getSize() const
{ return m_size; }

void BoidsManager::setSize(const float size)
{ m_size = size; }
