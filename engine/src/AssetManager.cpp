#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include <memory>

#include "ThirdParty/cereal/types/map.hpp"
#include "ThirdParty/cereal/types/vector.hpp"
#include "ThirdParty/cereal/types/memory.hpp"
#include "ThirdParty/cereal/archives/binary.hpp"

#include "AssetManager.hpp"
#include "Asset.hpp"
#include "AssetFactory.hpp"
#include "base64.hpp"
#include "Assets.hpp"

#include "ThirdParty/easylogging/easylogging++.h"

namespace ck
{

AssetFile::AssetFile()
{
    type = "NULL";
    a = nullptr;
}

void null_del(Asset *a){};

AssetFile::AssetFile(std::string t, Asset *asset)
{
    type = t;
    empty_delete<Asset> ed;
    a = std::shared_ptr<Asset>(asset, ed);
}

Asset *AssetFile::asset()
{
    return a.get();
};

template <class Archive>
void AssetFile::serialize(Archive &ar)
{
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
    }
    reset();
    std::ifstream is(fname, std::ios::binary);
    //is.open(fname);
    {
        cereal::BinaryInputArchive archive(is);
        archive(map);
    }
}

std::map<std::string, AssetFile> AssetManager::getMap()
{
    return map;
};

bool AssetManager::saveAsset(std::string name, Asset *asset)
{
    empty_delete<Asset> ed;
    //std::shared_ptr<Asset> sptr(asset, ed);
    std::cout << "saving asset of type: " << asset->getType() << std::endl;
    AssetFile file(asset->getType(), asset);
    std::cout << "made file" << std::endl;
    map[name] = file;
    //sptr.reset();
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
        std::cout << "File '" << fname << "' already exits, removing it" << std::endl;
        std::remove(fname.c_str());
    }
    std::cout << "saving and closing..." << std::endl;
    std::ofstream os(fname, std::ios::binary);
    os.open(fname);
    {
        cereal::BinaryOutputArchive archive(os);
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
};*/

bool AssetManager::fileExists(std::string fileName)
{
    std::ifstream infile(fileName);
    bool o = infile.good();
    infile.close();
    return o;
};
}