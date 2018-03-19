#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "AssetManager.hpp"
#include "Asset.hpp"
#include "AssetFactory.hpp"
#include "base64.hpp"
#include "Shader.hpp"

namespace ck
{

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

Asset *AssetManager::loadAsset(std::string name)
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
    /*if (data.substr(data.length() - 1, data.length()) == "\n")
    {
        data = data.substr(data.length() - 1, data.length());
    }*/
    asset->deserialize(data);
};

bool AssetManager::saveAsset(Asset *asset, std::string name)
{
    std::ofstream file;
    file.open(name);
    file << "[ TYPE " << asset->getType() << " VER " << asset->getVersion() << " ]" << std::endl;
    file << asset->serialize() << std::flush;
    file.close();
};

bool AssetManager::fileExists(std::string fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
};
}