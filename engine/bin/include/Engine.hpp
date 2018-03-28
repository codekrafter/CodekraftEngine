#pragma once

#include "EngineApp.hpp"
namespace ckg
{
extern ck::CKEngine *g_engine;
}
ck::CKEngine *initEngine(bool makeWindow);

int run(ck::CKEngine *engine);

ck::CKEngine *getEngine();