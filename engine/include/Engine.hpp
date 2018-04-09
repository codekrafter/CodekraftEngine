#pragma once

#include "EngineApp.hpp"
#include "ThirdParty/easylogging/easylogging++.h"
#include "Config.hpp"

namespace ck
{
extern CKEngine *engine;

CKEngine *initEngine(EngineConfig config);

int run(ck::CKEngine *engine);

CKEngine *getEngine();
}