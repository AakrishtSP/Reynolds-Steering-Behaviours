// src/Window/ImguiWindow.h
#pragma once
#include "imgui.h"
#include "Window.h"

class ImguiWindow {
public:
    ImguiWindow();
    ~ImguiWindow();

    void init(Window* window);
    void updateStart() const;
    void updateEnd() const;
    static void shutdown();

    [[nodiscard]] ImGuiIO* getIO() const { return m_io; }
private:
    ImGuiIO* m_io;
    Window* m_window;
};
