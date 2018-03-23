#include <string>
#include "Level.hpp"
#include "AssetManager.hpp"

namespace ck
{
Level::Level()
{
    version = 1;
    type = "LEVEL";
};
Level::~Level()
{
    for (std::shared_ptr<Actor> sp : contents)
    {
        sp->onDestroyed();
        delete sp.get();
        sp = nullptr;
    }
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
    ar(contents);
};

void Level::tick(float dt)
{
    for (std::shared_ptr<Actor> a : contents)
    {
        a->onTick(dt);
    }
}
}