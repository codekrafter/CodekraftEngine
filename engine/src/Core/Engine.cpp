#include <sstream>
#include <iostream>

#include "EngineConfig.hpp"
#include "Core/Engine.hpp"
#include "Core/EngineApp.hpp"
#include "colormod.h"
#include "Rendering/Display.hpp"
#include "Logging.hpp"

namespace ck
{
ck::CKEngine *engine = nullptr;

ck::CKEngine *initEngine(EngineConfig config)
{
    ck::term::color::Modifier red(ck::term::color::Code::FG_RED);
    ck::term::color::Modifier def(ck::term::color::Code::FG_DEFAULT);
    std::string format = "[%datetime] [%level] %msg";
    std::ostringstream ss;
    ss << red;
    ss << format;
    ss << def;
    std::string redFormat = ss.str();
    ck::CKEngine *app = new ck::CKEngine(config);
    ck::engine = app;
    LOG(INFO) << "Starting Codekraft Engine Version: " << ENGINE_VERSION << std::endl;
    return app;
}

int run(ck::CKEngine *engine)
{
    engine->getDisplay()->showWindow();
    while (!engine->getDisplay()->shouldClose())
    {
        engine->update();
    }
    return 0;
}

ck::CKEngine *getEngine()
{
    return ck::engine;
}
}