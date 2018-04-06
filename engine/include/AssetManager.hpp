#pragma once

#include "Asset.hpp"
#include <vector>
#include <string>
#include "Assets.hpp"

#include "ThirdParty/cereal/types/vector.hpp"
#include "ThirdParty/cereal/types/memory.hpp"
#include "ThirdParty/cereal/types/polymorphic.hpp"

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
    LOG(ERROR) << "\\\\\\\\\\\\\\\\\\ Operator () on ed of type: " << typeid(T).name();
  }
};
struct AssetFile
{
private:
  std::shared_ptr<Asset> a;

public:
  Asset *asset();
  std::shared_ptr<Asset> getAsset() { return a; };
  std::string type;
  std::shared_ptr<Asset> getAsset() { return a; };
  AssetFile();
  AssetFile(std::string t, Asset *asset);
  AssetFile(std::string t, std::shared_ptr<Asset> asset);
  ~AssetFile(){};
  template <class Archive>
  void serialize(Archive &ar);
  bool operator==(const AssetFile &other)
  {
    return a.get() == other.a.get();
  }
};
class AssetManager
{
private:
  static bool fileExists(std::string fileName);
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
    //A *out = reinterpret_cast<A *>(map[name].asset());
    A *out = dynamic_cast<A *>(map[name].asset());
    return out;
  }

  template <class A>
  std::shared_ptr<A> loadAssetSPTR(std::string name)
  {
    static_assert(std::is_base_of<Asset, A>::value, "A must be an asset");
    AssetFile af = map[name];
    return std::dynamic_pointer_cast<A>(af.getAsset());
  }
  bool saveAsset(std::string name, Asset *asset);
  std::vector<std::string> getKeys();
  std::map<std::string, AssetFile> getMap();
  /// Save map to file
  void close(std::string fname);
  void close();
};
}