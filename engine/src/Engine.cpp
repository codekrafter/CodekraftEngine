#include "EngineConfig.hpp"
#include "Engine.hpp"
#include "EngineApp.hpp"
#include "colormod.h"
#include "Display.hpp"

//#include "ThirdParty/glad/glad.h"
//#include "GLFW/glfw3.h"
//#include "ThirdParty/easylogging/easylogging++.h"

INITIALIZE_EASYLOGGINGPP

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
    el::Configurations conf;
    conf.setToDefault();
    conf.set(el::Level::Global,
             el::ConfigurationType::Format, format);
    conf.set(el::Level::Error,
             el::ConfigurationType::Format, redFormat);
    conf.set(el::Level::Fatal,
             el::ConfigurationType::Format, redFormat);
    conf.set(el::Level::Global, el::ConfigurationType::Filename, "./logs/ckengine.log");
    conf.set(el::Level::Global, el::ConfigurationType::ToFile, "true");
    conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "true");
    el::Loggers::reconfigureLogger("default", conf);
    LOG(INFO) << "Starting Engine Version: " << ENGINE_VERSION << std::endl;
    ck::CKEngine *app = new ck::CKEngine(config);
    ck::engine = app;
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