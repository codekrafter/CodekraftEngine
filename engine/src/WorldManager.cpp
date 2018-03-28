#include "WorldManager.hpp"

namespace ck
{
WorldManager *WorldManager::inst;

WorldManager::WorldManager()
{
    level = nullptr;
};
WorldManager::~WorldManager()
{
    delete level;
    level = nullptr;
};

WorldManager *WorldManager::getInstance()
{
    if (inst == nullptr)
    {
        inst = new WorldManager();
    }
    return inst;
};

void WorldManager::destroyInstance()
{
    if (inst == nullptr)
    {
        return;
    }
    else
    {
        delete inst;
        inst = nullptr;
        return;
    }
};

Level *WorldManager::getLevel()
{
    return level;
};
void WorldManager::loadLevel(Level *l)
{
    if (level != nullptr)
    {
        delete level;
        level = nullptr;
    }
    level = l;
};
}