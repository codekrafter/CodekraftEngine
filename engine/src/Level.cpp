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
Level::~Level(){

};
void Level::init(){};
/*void Level::deserialize(std::string data)
{
    std::vector<std::string> lines = AssetManager::split_string(data, "\n");
    std::string sel = "";
    for (std::string line : lines)
    {
        if (line.substr(0, 2) == "!!")
        {
            sel = line.substr(2, line.length());
        }
        else
        {
            if (sel == "")
            {
                if (line != "")
                {
                    throw std::runtime_error("Level Error: Content '" + line + "'appeared before section selector");
                }
            }
            else if (sel == "VERTEX")
            {
                *vertex = *vertex + line + "\n";
            }
            else if (sel == "FRAGMENT")
            {
                *fragment = *fragment + line + "\n";
            }
            else if (sel == "GEOMETRY")
            {
                *geometry = *geometry + line + "\n";
            }
            else
            {
                throw std::runtime_error("Level Error: Invalid section type: '" + sel + "'");
            }
        }
    };
};
std::string Level::serialize(){

};*/
std::string Level::getType()
{
    return type;
};
int Level::getVersion()
{
    return version;
};
}