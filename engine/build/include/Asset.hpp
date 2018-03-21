#pragma once

#include <string>
#include <TP/cereal/types/memory.hpp>
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
    version = a.version;
    type = a.type;
  }
  virtual ~Asset(){};
  template <class Archive>
  void serialize(Archive &ar) { ar(version, type); };
  virtual std::string getType() { return type; };
  /// Get version, should be an integer. Used in header
  virtual int getVersion() { return version; };
};
}