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
<<<<<<< HEAD
    switch (c.display.type)
    {
    case DisplayType::OPENGL:
        display = new opengl::DisplayOpenGL(c.display);
    }
=======
    display = new Display(c.display);
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba
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