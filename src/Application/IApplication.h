// src/Application/IApplication.h
#pragma once

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void init() = 0;
    virtual void onUpdate(float deltaTime) = 0;
    virtual void onImguiUpdate(float deltaTime) = 0;
    virtual void shutdown() = 0;
};
