#include <stdexcept>
#include <iostream>

#include "Asset.hpp"
#include "Shader.hpp"
#include "AssetFactory.hpp"

namespace ck
{
Asset* AssetFactory::create(std::string type)
{
    if (type == "SHADER")
    {
        std::cout << "returning new shader" << std::endl;
        return new Shader();
    }
    else
    {
        throw std::runtime_error("Asset Type '" + type + "' Invalid");
    }
};
}