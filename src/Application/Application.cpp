// src/Application/Application.cpp
#include "Application.h"
#include "imgui.h"
#include "Core/Boids/Agent.h"
// Core ko files haru include

void Application::init()
{
    // Aila bois manager instaniate huda bittikai tyo boid instantiate vaxa, which we do not really want right?
    m_BoidsManager = std::make_unique<BoidsManager>();
}

// This is for eveytime application update huda kheri !?
void Application::onUpdate(float deltaTime)
{
    m_BoidsManager->update();
}


void Application::onImguiUpdate(float deltaTime)
{
    ImGui::Begin("Boids Info and Spawner");

    ImGui::DragFloat2("Spawn Position", &m_spawnPosition.x, 1.0f);

    // Spawn Buttons
    if (ImGui::Button("Spawn Agent"))
    {
        m_BoidsManager->addBoid(std::make_unique<Agent>(m_spawnPosition));
    }
    ImGui::SameLine();
    if (ImGui::Button("Spawn Target"))
    {
        // TODO: Add logic to spawn a boid of type Target
    }
    ImGui::SameLine();
    if (ImGui::Button("Spawn Threat"))
    {
        // TODO: Add logic to spawn a boid of type Threat
    }


    // Collapsible header for the whole boids tree
    if (ImGui::CollapsingHeader("Boids Info"))
    {
        // Group boids by their type (using m_name)
        std::unordered_map<std::string, std::vector<Boid*>> boidsByType;
        for (const auto& boidPtr : m_BoidsManager->getBoids())
        {
            boidsByType[boidPtr->getName()].push_back(boidPtr.get());
        }

        // For each boid type group
        for (const auto& [name, boids] : boidsByType)
        {
            if (ImGui::TreeNode(name.c_str()))
            {
                // List each boid in the group
                for (const auto boid : boids)
                {
                    char boidLabel[64];
                    std::snprintf(boidLabel, sizeof(boidLabel), "%s - %p", boid->getName().c_str(), boid);

                    if (ImGui::TreeNode(boidLabel))
                    {
                        // Display boid properties
                        ImGui::Text("Position: (%.2f, %.2f)", boid->getPosition().x, boid->getPosition().y);
                        ImGui::Text("Velocity: (%.2f, %.2f)", boid->getVelocity().x, boid->getVelocity().y);
                        ImGui::Text("Acceleration: (%.2f, %.2f)", boid->getAcceleration().x, boid->getAcceleration().y);
                        ImGui::TreePop(); // End boid node
                    }
                }
                ImGui::TreePop(); // End type group node
            }
        }
    }
    ImGui::End();
}

void Application::shutdown()
{
}
