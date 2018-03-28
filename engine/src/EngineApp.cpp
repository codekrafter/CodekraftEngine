#include <iostream>

#include "EngineApp.hpp"
#include "Display.hpp"
#include "TP/easylogging/easylogging++.h"

namespace ck
{
CKEngine::CKEngine(bool mw)
{
    makeWindow = mw;
    display = new Display(!makeWindow);
    el::Configurations conf;
    conf.setToDefault();
    conf.set(el::Level::Global,
             el::ConfigurationType::Format, "[%datetime] [%level] %msg");
    conf.set(el::Level::Global, el::ConfigurationType::Filename, "./logs/ckengine.log");
    conf.set(el::Level::Global, el::ConfigurationType::ToFile, "true");
    conf.set(el::Level::Global, el::ConfigurationType::ToStandardOutput, "true");
    el::Loggers::reconfigureLogger("default", conf);
    LOG(INFO) << "started logging";
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