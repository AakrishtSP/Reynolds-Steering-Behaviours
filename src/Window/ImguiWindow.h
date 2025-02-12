// src/Window/ImguiWindow.h
#pragma once
#include "imgui.h"

class ImguiWindow {
public:
    ImguiWindow();
    ~ImguiWindow();

    void init();
    void updateStart();
    void updateEnd() const;
    void shutdown() const;
private:
   ImGuiIO& io;
};
