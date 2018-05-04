#pragma once

namespace ck
{
enum class DisplayType
{
    OPENGL
};
struct DisplayConfig
{
    DisplayType type;
    DisplayConfig() : DisplayConfig(DisplayType::OPENGL){};
    DisplayConfig(DisplayType type) : type(type){};
};
struct EngineConfig
{
    DisplayConfig display;
    EngineConfig() : display(){};
};
}