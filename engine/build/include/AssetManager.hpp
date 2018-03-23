#pragma once

#include "Asset.hpp"
#include <vector>
#include <string>
#include "TP/cereal/types/vector.hpp"
#include "TP/cereal/types/memory.hpp"

namespace ck
{
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
  std::string cname;

public:
  static std::vector<std::string> split_string(const std::string &str,
                                               const std::string &delimiter);
  AssetManager();
  ~AssetManager();
  /// Open file for loading
  void open(std::string fname);
  /// Resets internal asset map
  void reset();
  template <class A>
  A *loadAsset(std::string name)
  {
    static_assert(std::is_base_of<Asset, A>::value, "A must be an asset");
    AssetFile af = map[name];
    return dynamic_cast<A *>(af.asset.get());
  }
  bool saveAsset(std::string name, Asset *asset);
  std::vector<std::string> getKeys();
  /// Save map to file
  void close(std::string fname);
  void close();
};
}