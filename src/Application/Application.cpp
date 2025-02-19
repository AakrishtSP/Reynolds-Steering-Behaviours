// src/Application/Application.cpp
#include "Application.h"

#include "imgui.h"
#include "Core/Boids/Agent.h"
#include "GLFW/glfw3.h"
// Core ko files haru include

void Application::init()
{
    // Aila bois manager instaniate huda bittikai tyo boid instantiate vaxa, which we do not really want right?
    m_BoidsManager = std::make_unique<BoidsManager>();
}

// This is for eveytime application update huda kheri !?
void Application::onUpdate(float deltaTime)
{
    if (m_isSpawning && !m_onCooldown && glfwGetMouseButton(m_Window->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
        && !m_ImguiWindow->getIO()->WantCaptureMouse)
    {
        m_onCooldown = true;
        double xpos, ypos;
        glfwGetCursorPos(m_Window->getWindow(), &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(m_Window->getWindow(), &width, &height);

        m_spawnPosition = glm::vec2(xpos / width * 1280 - 640, -(ypos / height * 720 - 360)) - m_translation;
        m_BoidsManager->addBoid(std::make_unique<Agent>(m_spawnPosition));
    }
    if (m_onCooldown && glfwGetMouseButton(m_Window->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        m_onCooldown = false;
    }

    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_W) ==
        GLFW_REPEAT)
    {
        m_translation.y -= 10;
    }
    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_S) ==
        GLFW_REPEAT)
    {
        m_translation.y += 10;
    }
    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_A) ==
        GLFW_REPEAT)
    {
        m_translation.x += 10;
    }
    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_D) ==
        GLFW_REPEAT)
    {
        m_translation.x -= 10;
    }

    m_BoidsManager->update();

    m_BoidsManager->getDraw().setTranslation(m_translation);

    m_BoidsManager->getStates().setInfluenceFactor(m_influenceFactor);
    m_BoidsManager->getStates().setAgentTerminalSpeed(m_agentTerminalSpeed);
    m_BoidsManager->getStates().setInfluenceRadius(m_radiusOfInfluence);
    m_BoidsManager->getStates().setRandomnessFactor(m_randomnessFactor);
    m_BoidsManager->setSize(m_size);
}


void Application::onImguiUpdate(float deltaTime)
{
    ImGui::Begin("Boids Attributes and Spawner");
    ImGui::DragFloat2("Transform X", &m_translation.x, 1.0f);
    // ImGui::DragFloat2("Spawn Position", &m_spawnPosition.x, 1.0f);
    ImGui::Checkbox("Spawn Agent on click", &m_isSpawning);


    ImGui::SliderFloat("Raduius of Influence", &m_radiusOfInfluence, 0.0f, 200.0f);
    ImGui::SliderFloat("Influence Factor", &m_influenceFactor, 0.01f, 1.0f);
    ImGui::SliderFloat("Terminal Velocity", &m_agentTerminalSpeed, 1.0f, 10.0f);
    ImGui::SliderFloat("Randomness Factor", &m_randomnessFactor, 0.0f, 1.0f);
    ImGui::SliderFloat("Size", &m_size, 10.0f, 100.0f);

    if (ImGui::Button("Clear Boids"))
    {
        m_BoidsManager->setBoids({});
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Parameters"))
    {
        m_influenceFactor = 0.9;
        m_agentTerminalSpeed = 3.0f;
        m_radiusOfInfluence = 80;
        m_randomnessFactor = 0.2;
        m_translation = {0, 0};
        m_size = 30;
    }
    ImGui::Text("Current FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Current Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::End();


    if (ImGui::Begin("Obstacle"))
    {
        ImGui::Text("Obstacle Bounds");
        m_BoidsManager->getDraw().drawObstacleOutline(m_obstacleBounds, {1.0f,1.0f,1.0f}, 1);

        ImGui::SliderFloat("Top", &m_obstacleBounds.y, -360.0f, 360.0f);
        ImGui::SliderFloat("Bottom", &m_obstacleBounds.w, -360.0f, 360.0f);
        ImGui::SliderFloat("Left", &m_obstacleBounds.x, -640.0f, 640.0f);
        ImGui::SliderFloat("Right", &m_obstacleBounds.z, -640.0f, 640.0f);

        if (ImGui::Button("Add Obstacles"))
        {
            m_BoidsManager->defineObstracle(m_obstacleBounds);
        }
        ImGui::SameLine();
        if (ImGui::Button("Clear Obstacles"))
        {
            m_BoidsManager->setObstacles({});
            m_obstacleBounds = {-1280 / 2, 720 / 2, 1280 / 2, -720 / 2};
            m_BoidsManager->defineObstracle(m_obstacleBounds);
        }
    }
    ImGui::End();

    if (ImGui::Begin("Bois Info"))

    {
        ImGui::Text("Number of boids: %zu", m_BoidsManager->getBoids().size());
        ImGui::Checkbox("Show All Influence Circle", &showAllInfluenceCircle);
        // Group boids by their type (using m_name)
        std::unordered_map<std::string, std::vector<Boid*>> boidsByType;
        for (const auto& boidPtr : m_BoidsManager->getBoids())
        {
            if (showAllInfluenceCircle)
            {
                m_BoidsManager->getDraw().drawCircle(boidPtr->getPosition(),
                                                             m_BoidsManager->getStates().getInfluenceRadius());
            }
            boidsByType[boidPtr->getName()].push_back(boidPtr.get());
        }

        // For each boid type group
        for (const auto& [name, boids] : boidsByType)
        {
            if (ImGui::TreeNode(name.c_str()))
            {
                ImGui::Text("Number of boids: %zu", boids.size());
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
                        ImGui::Text("Number of neighbors: %zu", boid->getNeighbors().size());
                        ImGui::TreePop(); // End boid node
                        if (!showAllInfluenceCircle)
                        {
                            m_BoidsManager->getDraw().drawCircle(boid->getPosition(),
                                                                 m_BoidsManager->getStates().getInfluenceRadius());
                        }
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
