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
public:
  std::vector<std::shared_ptr<Actor>> contents;
  Level();
  virtual ~Level();
  void init();
  template <class Archive>
  void serialize(Archive &ar);
  void tick(float dt);
};
}