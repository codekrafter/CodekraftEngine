#include "ResourceManager.hpp"

namespace ck
{

void ResourceManager::deleteResources()
{
    if (phong != nullptr)
    {
        delete phong;
        phong = nullptr;
    }
};

Shader *ResourceManager::getPhong()
{
    if (phong == nullptr)
    {
        AssetManager *am = new AssetManager();
        am->open("shaders.ckd");
        phong = am->loadAsset<Shader>("phong");
        delete am;
        am = nullptr;
    }
    return phong;
};
}