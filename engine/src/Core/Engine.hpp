#pragma once

#include "Core/EngineApp.hpp"
//#include "Core/Config.hpp"

#define CKClass()
#define CKProperty(Arg, ...)
#define CKEvent()

namespace ck
{

struct EngineConfig;
class CKEngine;

extern CKEngine *engine;

CKEngine *initEngine(EngineConfig config);

int run(ck::CKEngine *engine);

CKEngine *getEngine();
}