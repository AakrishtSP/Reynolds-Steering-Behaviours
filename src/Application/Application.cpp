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
    if (m_isSpawning && !m_onCooldown && glfwGetMouseButton(m_Window->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !m_ImguiWindow->getIO()->WantCaptureMouse)
    {
        m_onCooldown = true;
        double xpos, ypos;
        glfwGetCursorPos(m_Window->getWindow(), &xpos, &ypos);

        int width, height;
        glfwGetWindowSize(m_Window->getWindow(), &width, &height);

        m_spawnPosition = glm::vec2(xpos/width * 1280 - 640, -(ypos/height * 720 - 360)) - m_translation;
        m_BoidsManager->addBoid(std::make_unique<Agent>(m_spawnPosition));
    }
    if (m_onCooldown && glfwGetMouseButton(m_Window->getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        m_onCooldown = false;
    }

    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_W) == GLFW_REPEAT)
    {
        m_translation.y -= 10;
    }
    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_S) == GLFW_REPEAT)
    {
        m_translation.y += 10;
    }
    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_A) == GLFW_REPEAT)
    {
        m_translation.x += 10;
    }
    if (glfwGetKey(m_Window->getWindow(), GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(m_Window->getWindow(), GLFW_KEY_D) == GLFW_REPEAT)
    {
        m_translation.x -= 10;
    }

    m_BoidsManager->update();

    m_BoidsManager->getDraw().setTranslation(m_translation);
}


void Application::onImguiUpdate(float deltaTime)
{
    ImGui::Begin("Transform");
    ImGui::DragFloat2("Transform X", &m_translation.x, 1.0f);
    ImGui::End();

    ImGui::Begin("Boids Info and Spawner");
    ImGui::DragFloat2("Spawn Position", &m_spawnPosition.x, 1.0f);
    ImGui::Checkbox("Spawn Agent on click", &m_isSpawning);

    // Collapsible header for the whole boids tree
    if (ImGui::CollapsingHeader("Boids Info"))
    {
        ImGui::Text("Number of boids: %zu", m_BoidsManager->getBoids().size());
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
                ImGui::SliderFloat("Raduius of Influence", &m_radiusOfInfluence, 0.0f, 200.0f);
                ImGui::Text("Number of boids: %zu", boids.size());
                // List each boid in the group
                for (const auto boid : boids)
                {
                    boid->setInfuenceRadius(m_radiusOfInfluence);
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
                        m_BoidsManager->getDraw().drawCircle(boid->getPosition(), boid->getInfuenceRadius());
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
