// src/Application/Application.cpp
#include "Application.h"
#include "imgui.h"
// Core ko files haru include

void Application::init()
{
    m_BoidsManager = std::make_unique<BoidsManager>();
}

void Application::onUpdate(float deltaTime)
{
    m_BoidsManager->update();
}

void Application::onImguiUpdate(float deltaTime)
{
    ImGui::Text("This is a test window.");
}

void Application::shutdown()
{
}
