#pragma once

<<<<<<< HEAD
#include "EngineApp.hpp"
#include "ThirdParty/easylogging/easylogging++.h"
#include "Config.hpp"

namespace ck
{
extern CKEngine *engine;

CKEngine *initEngine(EngineConfig config);

int run(ck::CKEngine *engine);

CKEngine *getEngine();
=======
//#include "EngineApp.hpp"

namespace ck
{

class CKEngine;

enum class DisplayType
{
    OPENGL
};
struct DisplayConfig
{
    DisplayType type;
    DisplayConfig(DisplayType type) : type(type) {};
};
struct EngineConfig
{
    DisplayConfig display;
};

extern CKEngine *engine;

ck::CKEngine *initEngine(EngineConfig config);

int run(ck::CKEngine *engine);

ck::CKEngine *getEngine();
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba
}