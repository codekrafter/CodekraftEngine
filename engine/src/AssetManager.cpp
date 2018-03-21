#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <type_traits>

#include "TP/cereal/types/unordered_map.hpp"
#include "TP/cereal/types/memory.hpp"
#include "TP/cereal/archives/binary.hpp"

#include "AssetManager.hpp"
#include "Asset.hpp"
#include "AssetFactory.hpp"
#include "base64.hpp"
#include "Shader.hpp"
#include "StaticMesh.hpp"

namespace ck
{

AssetFile::AssetFile()
{
    type = "NULL";
    asset = nullptr;
}
AssetFile::AssetFile(std::string t, std::shared_ptr<Asset> &a)
{
    type = t;
    asset = a;
}

template <class Archive>
void AssetFile::serialize(Archive &ar)
{
    ar(type, asset);
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

AssetManager::AssetManager(){};

AssetManager::~AssetManager(){};

/*Asset *AssetManager::loadAsset(std::string name)
{
    std::ifstream file;
    file.open(name);
    std::string line;
    std::string data;
    std::string type;
    int version;
    // Next Type
    std::string nt;
    while (std::getline(file, line))
    {
        if (line.c_str()[0] == '[')
        {
            std::vector<std::string> tokens = split_string(line, " ");
            for (std::string token : tokens)
            {
                if (token == "[")
                {
                    nt = "";
                }
                else if (token == "]")
                {
                    nt = "";
                    break;
                }
                else if (nt != "")
                {
                    if (nt == "TYPE")
                    {
                        type = token;
                        nt = "";
                    }
                    else if (nt == "VER")
                    {
                        std::stringstream ss(token);
                        int i = 0;
                        ss >> i;
                        version = i;
                        nt = "";
                    }
                    else
                    {
                        throw std::runtime_error("Unknown header value type '" + nt + "'");
                    }
                }
                else if (token == "TYPE")
                {
                    nt = "TYPE";
                }
                else if (token == "VER")
                {
                    nt = "VER";
                }
                else
                {
                    throw std::runtime_error("unknown token '" + token + "' while parsing asset header");
                };
            }
        }
        else
        {
            data = data + line + "\n";
        }
    }
    file.close();
    Asset *asset = AssetFactory::create(type);
    if (data.substr(data.length() - 1, data.length()) == "\n")
    {
        data = data.substr(0, data.length()-1);
    }
    asset->deserialize(data);
    return asset;
};*/

AssetFile AssetManager::loadAsset(std::string name)
{
    std::ifstream is(name, std::ios::binary);
    cereal::BinaryInputArchive archive(is);
    AssetFile file;
    archive(file);
    return file;
}

template <typename T>
struct empty_delete
{
    empty_delete() /* noexcept */
    {
    }

    template <typename U>
    empty_delete(const empty_delete<U> &,
                 typename std::enable_if<
                     std::is_convertible<U *, T *>::value>::type * = nullptr) /* noexcept */
    {
    }

    void operator()(T *const) const /* noexcept */
    {
        // do nothing
    }
};

bool AssetManager::saveAsset(Asset *asset, std::string name)
{
    /*if (fileExists(name))
    {
        std::cout << "File '" << name << "' already exits, removing it" << std::endl;
        std::remove(name.c_str());
    }*/
    std::ofstream os(name, std::ios::binary);
    empty_delete<Asset> ed;
    std::shared_ptr<Asset> sptr(asset, ed);
    std::cout << "saving asset of type: " << asset->getType() << std::endl;
    AssetFile file(asset->getType(), sptr);
    {
        cereal::BinaryOutputArchive archive(os);
        archive(file);
    }
    sptr.reset();
}

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
    return infile.good();
};
}