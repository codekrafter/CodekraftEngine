#pragma once

#include "Asset.hpp"

//#include <glad/glad.h>
//#include <TP/glm/glm.hpp>

#include "Asset.hpp"
namespace ck
{
class Level : public Asset
{
private:

public:
  Level();
  virtual ~Level();
  void init();
  virtual void deserialize(std::string data);
  virtual std::string serialize();
  virtual std::string getType();
  virtual int getVersion();
};
}