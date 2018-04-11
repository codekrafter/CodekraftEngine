#pragma once

#include <ThirdParty/glm/glm.hpp>
//#include <ThirdParty/cereal/types/memory.hpp>
#include "GLMSavers.hpp"

namespace ck
{
struct Transform
{
public:
  template <class Archive>
  void serialize(Archive &ar)
  {
    ar(location, rotation, scale);
  };
  glm::vec3 location = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
}