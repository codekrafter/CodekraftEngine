#include <iostream>
#include "EngineConfig.hpp"
#include "Engine.hpp"
#include "EngineApp.hpp"

//#include "ThirdParty/glad/glad.h"
//#include "GLFW/glfw3.h"
#include "ThirdParty/easylogging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

namespace ckg
{
ck::CKEngine *g_engine = nullptr;
}

ck::CKEngine *initEngine(bool makeWindow)
{
    std::cout << "Starting Engine Version: " << ENGINE_VERSION << std::endl;
    ck::CKEngine *app = new ck::CKEngine(makeWindow);
    ckg::g_engine = app;
    return app;
}

int run(ck::CKEngine *engine)
{
    glfwShowWindow(engine->getDisplay()->getWindow());
    while (!glfwWindowShouldClose(engine->getDisplay()->getWindow()))
    {
        engine->update();
    }
}

ck::CKEngine *getEngine()
{
    return ckg::g_engine;
}