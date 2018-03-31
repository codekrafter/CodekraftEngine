#pragma once

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
}