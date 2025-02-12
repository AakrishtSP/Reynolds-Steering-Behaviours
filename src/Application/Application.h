// src/Application/Application.h
#pragma once
#include "IApplication.h"

class Application : public IApplication {
public:
    void init() override;
    void onUpdate(float deltaTime) override;
    void onImguiUpdate(float deltaTime) override;
    void shutdown() override;
private:
};
