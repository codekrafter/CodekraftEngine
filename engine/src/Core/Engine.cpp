#include <sstream>
#include <iostream>

#include "EngineConfig.hpp"
#include "Core/Engine.hpp"
#include "Core/EngineApp.hpp"
#include "colormod.h"
#include "Rendering/Core/RenderingManager.hpp"
#include "Rendering/Core/Window.hpp"
#include "Logging.hpp"

namespace ck
{
ck::CKEngine *engine = nullptr;

ck::CKEngine *initEngine(EngineConfig config)
{
    ck::CKEngine *app = new ck::CKEngine(config);
    ck::engine = app;
    LOG(INFO) << "Starting Codekraft Engine Version: " << ENGINE_VERSION << std::endl;
    return app;
}

int run(ck::CKEngine *engine)
{
    engine->init();
    RenderingManager::inst()->getWindow()->setVisibility(true);
    while (!RenderingManager::inst()->getWindow()->shouldClose())
    {
        engine->update();
    }
    return 0;
}

ck::CKEngine *getEngine()
{
    return ck::engine;
}
} // namespace ck