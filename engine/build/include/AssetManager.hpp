#pragma once

#include "Asset.hpp"
#include <vector>
#include <string>

namespace ck
{
class AssetManager
{
private:
  bool fileExists(std::string fileName);

public:
  static std::vector<std::string> split_string(const std::string &str,
                                        const std::string &delimiter);
  AssetManager();
  ~AssetManager();
  Asset *loadAsset(std::string fileName);
  bool saveAsset(Asset *asset, std::string fileName);
};
}