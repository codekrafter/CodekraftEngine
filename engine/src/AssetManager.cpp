#include <string>
#include <algorithm>

#include "AssetManager.hpp"
#include "AssetStructures.hpp"

namespace ck
{

AssetManager *AssetManager::iinst;

AssetManager::AssetManager(){};

AssetManager::~AssetManager(){};

// Takes a string as an asset ID, and makes sure there is no whitespace, and it is all lowercase
std::string AssetManager::processID(std::string i)
{
    std::string ID = i;
    for (char c : ID) // Check to make sure there are no whitespace characters in the ID, this could cause problems down the road
    {
        assert(!isspace(c));
    }
    std::transform(ID.begin(), ID.end(), ID.begin(), ::tolower); // Make ID lowercase, again to prevent problems down the road
    return ID;
};

AssetManager *AssetManager::inst()
{
    if (iinst == nullptr)
    {
        return new AssetManager();
    }
    return iinst;
};

void AssetManager::cleanup()
{
    delete iinst;
    iinst = nullptr;
};

void AssetManager::flush()
{
    return;
};

void AssetManager::unload()
{
    for (std::pair<std::string, std::map<std::string, Asset *>> pair : map)
    {
        unload(pair.first);
    }
};

void AssetManager::unload(std::string name)
{
    flush();
};

void AssetManager::load()
{
    for (std::pair<std::string, std::map<std::string, Asset *>> pair : map)
    {
        unload(pair.first);
    }
};

void AssetManager::load(std::string name)
{
    flush();
};

void AssetManager::savef()
{
    return;
};
void AssetManager::loadf()
{
    return;
};

size_t AssetManager::getSize(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    if (name == "SHADER")
    {
        return sizeof(ShaderS);
    }
    else if (name == "MESH")
    {
        return sizeof(MeshS);
    }
    else if (name == "MATERIAL")
    {
        return sizeof(MaterialS);
    }
    else if (name == "TEXTURE")
    {
        return sizeof(TextureS);
    }
    else if (name == "STATIC_MESH")
    {
        return sizeof(StaticMeshS);
    }
    else
    {
        LOG(ERROR) << "Could not find asset for name: '" << name << "'";
        return 0;
    }
};
AssetS *AssetManager::getObject(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    if (name == "SHADER")
    {
        return new ShaderS();
    }
    else if (name == "MESH")
    {
        return new MeshS();
    }
    else if (name == "MATERIAL")
    {
        return new MaterialS();
    }
    else if (name == "TEXTURE")
    {
        return new TextureS();
    }
    else if (name == "STATIC_MESH")
    {
        return new StaticMeshS();
    }
    else
    {
        LOG(ERROR) << "Could not find asset for name: '" << name << "'";
        return nullptr;
    }
};
}