// src/Application/IApplication.h
#pragma once
#include "Window/ImguiWindow.h"
#include "Window/Window.h"

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void init() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onImguiUpdate(float deltaTime) = 0;
    virtual void shutdown() = 0;
    void setWindow(Window* window) { m_Window = window; }
    void setImguiWindow(ImguiWindow* imguiWindow) { m_ImguiWindow = imguiWindow; }
    [[nodiscard]] Window* getWindow() const {return m_Window;}
    [[nodiscard]] ImguiWindow* getImguiWindow() const {return m_ImguiWindow;}
protected:
    Window* m_Window{};
    ImguiWindow* m_ImguiWindow{};
};
