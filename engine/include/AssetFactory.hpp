#include "Asset.hpp"
namespace ck {
class AssetFactory
{
  public:
    static Asset *create(std::string type);
};
}