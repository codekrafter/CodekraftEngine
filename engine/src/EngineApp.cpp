#include <iostream>

#include "EngineApp.hpp"
#include "Display.hpp"
#include "ThirdParty/easylogging/easylogging++.h"

namespace ck
{
CKEngine::CKEngine(EngineConfig c)
{
    config = c;
    display = new Display(c.display);
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