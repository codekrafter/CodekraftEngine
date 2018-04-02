#pragma once

#include <string>
#include <ThirdParty/cereal/types/memory.hpp>
#include <ThirdParty/cereal/types/polymorphic.hpp>

namespace ck
{
class Asset
{
protected:
  int version;
  std::string type;

public:
  Asset()
  {
    version = 0;
    type = "GENERIC";
  };

  Asset(const Asset &a)
  {
    version = a.version;
    type = a.type;
  };

  virtual ~Asset(){};
  virtual void init() = 0;
  template <class Archive>
  void serialize(Archive &ar) { ar(version, type); };
  virtual std::string getType() { return type; };
  /// Get version, should be an integer. Used in header
  virtual int getVersion() { return version; };
};
}