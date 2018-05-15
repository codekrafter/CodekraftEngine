#pragma once

namespace ck
{
enum class DrawType
{
    OPENGL,
    VULKAN
};

enum class WindowType
{
    GLFW
};
struct RenderingConfig
{
    WindowType window;
    DrawType draw;
    int width = 800;
    int height = 600;
    RenderingConfig() : RenderingConfig(DrawType::OPENGL){};
    RenderingConfig(DrawType d) : RenderingConfig(d, WindowType::GLFW){};
    RenderingConfig(DrawType d, WindowType w) : window(w), draw(d){};

  private:
};
struct EngineConfig
{
    RenderingConfig rendering;
    EngineConfig() : rendering(){};
};
} // namespace ck