#pragma once

#include <string>
#include "Engine.hpp"
#include <ThirdParty/cereal/types/memory.hpp>
#include <ThirdParty/cereal/types/string.hpp>
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
    version = 1;
    type = "GENERIC";
  };

  Asset(const Asset &a)
  {
    LOG(DEBUG) << "Calling Copy Constructor for asset of type: " << typeid(a).name() << " && " << a.type;
    version = a.version;
    type = a.type;
  };

  virtual ~Asset(){};
  virtual void init() = 0;
  template <class Archive>
  void serialize(Archive &ar) { ar(version, type); };
  virtual std::string getType() { return type; };
  virtual int getVersion() { return version; };
};
}