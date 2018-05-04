#include <string>
#include <memory>

#include "Level.hpp"
#include "Assets/AssetManager.hpp"

namespace ck
{
Level::Level()
{
    version = 1;
    type = "LEVEL";
    contents = std::vector<Actor *>();
    cam = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
};
Level::~Level()
{
    for (Actor *p : contents)
    {
        p->onDestroyed();
        delete p;
        p = nullptr;
    }
    delete cam;
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

void Level::forwardDraw(float dt)
{
    if (!initialized)
    {
        this->init();
        initialized = true;
    }
    for (Actor *a : forward)
    {
        a->onTick(dt);
    }
};

Camera *Level::getCamera()
{
    return cam;
};

void Level::setCamera(Camera *c)
{
    if (cam != nullptr)
    {
        delete cam;
        cam = nullptr;
    }
    cam = c;
};
}