// src/Application/ApplicationManager.cpp
#include "ApplicationManager.h"

ApplicationManager::ApplicationManager(IApplication* application): m_Application(application)
{
    init();
}

ApplicationManager::~ApplicationManager()
{
    shutdown();
}

void ApplicationManager::init()
{
    m_Window = std::make_unique<Window>();
    m_Window->init();
    m_Application->init();
    update();
}

void ApplicationManager::update() const
{
    m_Window->updateStart();
    m_Application->update(0.0f);
    m_Window->updateEnd();
}

void ApplicationManager::shutdown() const
{
    m_Application->shutdown();
}
