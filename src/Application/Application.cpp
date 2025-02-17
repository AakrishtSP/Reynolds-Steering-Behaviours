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

    m_BoidsManager->addBoid(std::make_unique<Agent>());
    

}

// This is for eveytime application update huda kheri !?
void Application::onUpdate(float deltaTime)
{
    if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glm::vec2 spawnPosition(-100,-100);
        m_BoidsManager->addBoid(std::make_unique<Agent>(spawnPosition));
    }

    m_BoidsManager->update();
}


void Application::onImguiUpdate(float deltaTime)
{
    ImGui::Text("This is a test window.");
}

void Application::shutdown()
{
}
