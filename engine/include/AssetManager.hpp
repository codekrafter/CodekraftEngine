#pragma once

#include "Asset.hpp"
#include <vector>
#include <string>
#include "TP/cereal/types/vector.hpp"
#include "TP/cereal/types/memory.hpp"

namespace ck
{
struct AssetFile
{
  std::string type;
  std::shared_ptr<Asset> asset;
  AssetFile();
  AssetFile(std::string t, std::shared_ptr<Asset> &a);
  template <class Archive>
  void serialize(Archive &ar);
};
class AssetManager
{
private:
  bool fileExists(std::string fileName);
  std::map<std::string, AssetFile> map;


public:
  static std::vector<std::string> split_string(const std::string &str,
                                               const std::string &delimiter);
  AssetManager();
  ~AssetManager();
  /// Open file for loading
  void open(std::string fname);
  /// Resets internal asset map
  void reset();
  AssetFile loadAsset(std::string name);
  bool saveAsset(std::string name, Asset *asset);
  /// Save map to file
  void close(std::string fname);
};
}