// src/Application/ApplicationManager.h
#pragma once
#include "IApplication.h"
#include "Window/Window.h"
#include "Window/ImguiWindow.h"

class ApplicationManager {
public:
    explicit ApplicationManager(IApplication* application);
    ~ApplicationManager();

    void init();
    void update();
    void shutdown() const;

    [[nodiscard]] Window* getWindow() const { return m_Window; }

private:
    bool m_Running = true;
    IApplication* m_Application;
    Window* m_Window{};
    ImguiWindow* m_ImguiWindow{};
};
