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
        return new Shader();
    }
    else
    {
        throw std::runtime_error("Asset Type '" + type + "' Invalid");
    }
};
}