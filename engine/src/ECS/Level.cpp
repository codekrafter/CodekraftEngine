#include <string>
#include <memory>
#include <typeinfo>

#include "Level.hpp"
#include "Assets/AssetManager.hpp"
#include "Rendering/Core/RenderingManager.hpp"
#include "Camera/ACamera.hpp"

namespace ck
{
Level::Level()
{
    version = 1;
    type = "LEVEL";
    contents = std::vector<Actor *>();
};
Level::~Level()
{
    for (Actor *p : contents)
    {
        p->onDestroyed();
        delete p;
        p = nullptr;
    }
};

void Level::init()
{
    for (Actor *a : contents)
    {
        a->onBeginPlay();

        for (ActorComponent *ac : a->components)
        {
            ac->onBeginPlay();
        }
    }
};

void Level::tick(float dt)
{
    if (!initialized)
    {
        this->init();
        initialized = true;
    }
    return;
    for (Actor *a : contents)
    {
        a->render();

        for (ActorComponent *ac : a->components)
        {
            ac->render();
        }
    }
}

void Level::addActor(Actor *a)
{
    if (a != nullptr)
    {
        contents.push_back(a);
    }
    else
    {
        LOG(WARNING) << "Actor* is nullptr, not adding to scene graph";
    }
};

ACamera *Level::getCamera()
{
    const std::type_info &type_target = typeid(ACamera);
    auto it = find_if(contents.begin(), contents.end(), [=](const Actor *actor) { return typeid(*actor) == type_target; });

    if (*it == 0x0 || it == contents.end())
    {
        LOG(INFO) << "no camera, making one ";
        ACamera *cam = new ACamera();
        contents.push_back(cam);
        return cam;
    }
    else
    {
        return dynamic_cast<ACamera *>(*it);
    }
}
} // namespace ck