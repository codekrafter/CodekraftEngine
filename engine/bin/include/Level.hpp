#pragma once

#include "Asset.hpp"

//#include <glad/glad.h>
//#include <TP/glm/glm.hpp>

#include "Asset.hpp"
#include "Actor.hpp"

namespace ck
{
class Level : public Asset
{
private:
  std::vector<Actor> contents;

public:
  Level();
  virtual ~Level();
  void init();
  virtual std::string getType();
  template <class Archive>
  void serialize( Archive & ar );
  virtual int getVersion();
};
}