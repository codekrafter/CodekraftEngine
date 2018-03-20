#pragma once

#include "Asset.hpp"
#include <vector>
#include <string>

namespace ck
{
  struct AssetFile
{
    std::string type;
    std::shared_ptr<Asset> asset;
    AssetFile();
    AssetFile(std::string t,std::shared_ptr<Asset> a);
    template <class Archive>
    void serialize( Archive & ar );
};
class AssetManager
{
private:
  bool fileExists(std::string fileName);

public:
  static std::vector<std::string> split_string(const std::string &str,
                                        const std::string &delimiter);
  AssetManager();
  ~AssetManager();
  AssetFile loadAsset(std::string fileName);
  bool saveAsset(Asset *asset, std::string fileName);
};
}