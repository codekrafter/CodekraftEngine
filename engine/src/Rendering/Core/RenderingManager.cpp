#include "RenderingManager.hpp"

#include "Core/Engine.hpp"
#include "Rendering/GLFW/GLFWWindow.hpp"
#include "Rendering/OpenGL/OpenGLRenderer.hpp"
#include "Logging.hpp"

namespace ck
{
RenderingManager::RenderingManager(RenderingConfig rc)
{
    switch (rc.draw)
    {
    case DrawType::OPENGL:
        renderer = new OpenGLRenderer();
        break;
    case DrawType::VULKAN:
        LOG(INFO) << "Using Vulkan";
        break;
    default:
        LOG(FATAL) << "Unkown Draw Type: " << demangle(typeid(rc.draw).name());
    };

    switch (rc.window)
    {
    case WindowType::GLFW:
        window = new GLFWWindow(rc.draw);
        break;
    default:
        LOG(FATAL) << "Unkown Draw Type: " << demangle(typeid(rc.window).name());
    }
};

RenderingManager *RenderingManager::inst()
{
    return getEngine()->rm;
};

void RenderingManager::preRender()
{
    window->preRender();
    SceneInfo info;
    renderer->preRender(info);
};

void RenderingManager::postRender()
{
    renderer->postRender();
    window->postRender();
};

Renderer *RenderingManager::getRenderer()
{
    return renderer;
}
Window *RenderingManager::getWindow()
{
    return window;
}

} // namespace ck