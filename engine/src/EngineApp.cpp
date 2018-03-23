#include <iostream>

#include "EngineApp.hpp"
#include "Display.hpp"
namespace ck
{
CKEngine::CKEngine(bool mw)
{
    makeWindow = mw;
    display = new Display(!makeWindow);
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
}