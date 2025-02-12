// src/Application/ApplicationManager.h
#pragma once
#include "IApplication.h"
#include "Window/Window.h"

class ApplicationManager {
public:
    explicit ApplicationManager(IApplication* application);
    ~ApplicationManager();

    void init();
    void update() const;
    void shutdown() const;

    std::unique_ptr<Window>& getWindow() { return m_Window; }

private:
    std::unique_ptr<IApplication> m_Application;
    std::unique_ptr<Window> m_Window;
};
