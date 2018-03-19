#include <iostream>
#include "EngineConfig.hpp"
#include "Engine.hpp"
#include "EngineApp.hpp"

#include "TP/glad/glad.h"
#include "GLFW/glfw3.h"

CKEngine *initEngine()
{
    std::cout << "Starting Engine Version: " << ENGINE_VERSION << std::endl;
    CKEngine *app = new CKEngine();
    return app;
}

int run(CKEngine *engine)
{
    while (!glfwWindowShouldClose(engine->getDisplay()->getWindow()))
    {
        engine->update();
    }
}