/*#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include <memory>

#include "ThirdParty/cereal/types/map.hpp"
#include "ThirdParty/cereal/types/string.hpp"
#include "ThirdParty/cereal/types/vector.hpp"
#include "ThirdParty/cereal/types/memory.hpp"
#include "ThirdParty/cereal/archives/portable_binary.hpp"

#include "AssetManager.hpp"
#include "Asset.hpp"
#include "AssetFactory.hpp"
#include "base64.hpp"
#include "GenericAsset.hpp"
#include "Assets.hpp"


namespace ck
{

void empty_delete(Asset *){};

AssetFile::AssetFile()
{
    a = nullptr;
    type = "NULL";
}

AssetFile::AssetFile(std::string t, Asset *asset)
{
    if (asset == nullptr)
    {
        LOG(ERROR) << "!!!!!!!!!!!!!!!!!!Creating AssetFile with ptr == nullptr";
    }
    type = t;
    a = std::shared_ptr<Asset>(asset, empty_delete);
}

AssetFile::AssetFile(std::string t, std::shared_ptr<Asset> asset)
{
    if (asset == nullptr)
    {
        LOG(ERROR) << "!!!!!!!!!!!!!!!!!!Creating AssetFile with sptr == nullptr";
    }
    type = t;
    a = asset;
}

Asset *AssetFile::asset()
{
    return a.get();
};

template <class Archive>
void AssetFile::serialize(Archive &ar)
{
    if (a == nullptr)
    {
        LOG(ERROR) << "Tried to serialize AssetFile when asset sptr is nullptr";
        LOG(ERROR) << "THIS: " << this;
        LOG(ERROR) << "TYPE: " << type;
        LOG(ERROR) << "Asset(a): " << a;
    }
    ar(type, a);
}

std::vector<std::string> AssetManager::split_string(const std::string &str,
                                                    const std::string &delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

AssetManager::AssetManager()
{
    map = std::map<std::string, AssetFile>();
};

AssetManager::~AssetManager(){};

void AssetManager::open(std::string fname)
{
    if (!fileExists(fname))
    {
        LOG(ERROR) << "tried to open file '" << fname << "' that does not exist";
        return;
    }
    reset();
    std::ifstream is(fname, std::ios::binary);
    //is.open(fname);
    {
        cereal::PortableBinaryInputArchive archive(is);
        archive(map);
    }
}

std::map<std::string, AssetFile> AssetManager::getMap()
{
    return map;
};

bool AssetManager::saveAsset(std::string name, Asset *asset)
{
    std::shared_ptr<Asset> sptr(asset, empty_delete);
    AssetFile file(asset->getType(), sptr); //asset);
    map[name] = file;
    //sptr.reset();
    return true;
}

std::vector<std::string> AssetManager::getKeys()
{
    std::vector<std::string> out;
    for (std::pair<std::string, AssetFile> pair : map)
    {
        out.push_back(pair.first);
    }
    return out;
}

void AssetManager::reset()
{
    //map.clear();
    map = std::map<std::string, AssetFile>();
}
void AssetManager::close()
{
    close(cname);
}
void AssetManager::close(std::string fname)
{
    if (fileExists(fname))
    {
        std::remove(fname.c_str());
    }
    std::ofstream os(fname, std::ios::binary);
    os.open(fname);
    {
        cereal::PortableBinaryOutputArchive archive(os);
        archive(map);
    }
    os.close();
};

/*bool AssetManager::saveAsset(Asset *asset, std::string name)
{
    std::ofstream file;
    file.open(name);
    file << "[ TYPE " << asset->getType() << " VER " << asset->getVersion() << " ]" << std::endl;
    file << asset->serialize() << std::flush;
    file.close();
};/

bool AssetManager::fileExists(std::string fileName)
{
    std::ifstream infile(fileName);
    bool o = infile.good();
    infile.close();
    return o;
};
}*/