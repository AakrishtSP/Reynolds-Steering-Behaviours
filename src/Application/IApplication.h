// src/Application/IApplication.h
#pragma once
#include "Window/Window.h"

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void init() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onImguiUpdate(float deltaTime) = 0;
    virtual void shutdown() = 0;
    void setWindow(Window* window) { m_Window = window; }
protected:
    Window* m_Window{};
};
