#include <string>
#include <memory>

#include "Level.hpp"
#include "AssetManager.hpp"

namespace ck
{
Level::Level()
{
    version = 1;
    type = "LEVEL";
    contents = std::vector<std::shared_ptr<Actor>>();
    cam = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
};
Level::~Level()
{
    for (std::shared_ptr<Actor> sp : contents)
    {
        sp->onDestroyed();
        /*delete sp.get();
        sp = nullptr;*/
        sp.reset();
    }
    delete cam;
};

void Level::init()
{
    for (std::shared_ptr<Actor> a : contents)
    {
        a->onBeginPlay();
    }
};
template <class Archive>
void Level::serialize(Archive &ar)
{
    ar(/*cereal::base_class<ck::Asset>(this),*/ contents);
};

void Level::tick(float dt)
{
    if (!initialized)
    {
        this->init();
        initialized = true;
    }
    for (std::shared_ptr<Actor> a : contents)
    {
        a->onTick(dt);
    }
}
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