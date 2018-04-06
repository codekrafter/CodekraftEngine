#pragma once

#include "Asset.hpp"

namespace ck
{
class GenericAsset : public Asset
{

public:
  GenericAsset()
  {
    version = 1;
    type = "GENERIC";
  };

  virtual ~GenericAsset(){};
  virtual void init() {};
  //template <class Archive>
  //void serialize(Archive &ar) { ar(version, type); };
  //virtual std::string getType() { return type; };
  //virtual int getVersion() { return version; };
};
}