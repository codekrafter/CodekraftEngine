#pragma once

//#include "EngineApp.hpp"

namespace ck
{

class CKEngine;

enum class DisplayType
{
    OPENGL
};
struct EngineConfig
{
    DisplayType display;
};

extern CKEngine *engine;

ck::CKEngine *initEngine(EngineConfig config);

int run(ck::CKEngine *engine);

ck::CKEngine *getEngine();
}