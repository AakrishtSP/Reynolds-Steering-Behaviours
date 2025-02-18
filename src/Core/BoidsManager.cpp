// src/Core/BoidsManager.cpp
#include "BoidsManager.h"

#include "Renderer/Draw.h"

BoidsManager::BoidsManager()
{
    m_Draw.init();
}

BoidsManager::~BoidsManager()
= default;

void BoidsManager::update()
{
    //Do something here


    //yo vanda tala kolai abstract garda pani hunxa

    if (!m_boid.empty())
    {
        m_Draw.draw(m_boid);
        m_UpdateStates.updateBoidsDeafult(m_boid);
    }

    for (const auto& boid : m_boid)
    {
        boid->update(m_boid, m_UpdateStates);
    }
}

void BoidsManager::addBoid(std::unique_ptr<Boid> boid)
{
    m_boid.push_back(std::move(boid));
}


States& BoidsManager::getStates()
{
    return this->m_UpdateStates;
}