#include <iostream>

#include "EngineApp.hpp"
#include "Display.hpp"
#include "DisplayOpenGL.hpp"
#include "ThirdParty/easylogging/easylogging++.h"

namespace ck
{
CKEngine::CKEngine(EngineConfig c)
{
    config = c;
    switch (c.display.type)
    {
    case DisplayType::OPENGL:
        display = new opengl::DisplayOpenGL(c.display);
    }
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