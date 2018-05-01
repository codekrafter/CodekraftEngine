#pragma once

#include "EngineApp.hpp"
//#include "Config.hpp"

namespace ck
{

struct EngineConfig;
class CKEngine;

extern CKEngine *engine;

CKEngine *initEngine(EngineConfig config);

int run(ck::CKEngine *engine);

CKEngine *getEngine();
}