// src/Core/States/States.h
#pragma once



class States {
public:
    virtual void enter()=0;
    virtual void update()=0;
    virtual void exit()=0;
};
