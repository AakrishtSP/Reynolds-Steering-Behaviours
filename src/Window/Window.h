// src/Window/Window.h
#pragma once
#include <memory>
#include "GLFW/glfw3.h"

class Window {
public:
    Window();
    ~Window();

    void init();

    void updateStart();

    void updateEnd() const;

    void shutdown() const;

    [[nodiscard]] GLFWwindow* getWindow() const { return m_window; }

    [[nodiscard]] int getWidth() const { return m_width; }

    [[nodiscard]] int getHeight() const { return m_height; }

private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;
};
