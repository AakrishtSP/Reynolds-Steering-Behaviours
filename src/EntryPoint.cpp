// src/EntryPoint.cpp
#include "Application/Application.h"
#include "Application/ApplicationManager.h"


int main()
{
    ApplicationManager appManager(new Application());
    appManager.init();
    appManager.update();
    appManager.shutdown();
    return 0;
}