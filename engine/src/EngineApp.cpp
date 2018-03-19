#include <iostream>

#include "EngineApp.hpp"
#include "Display.hpp"

CKEngine::CKEngine()
{
    display = new Display();
};
CKEngine::~CKEngine()
{
    delete display;
    display = nullptr;
};
Display *CKEngine::getDisplay()
{
    return display;
};

void CKEngine::update()
{
    display->update();
}