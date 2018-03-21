#pragma once

#include <TP/glm/glm.hpp>

namespace ck
{
struct Transform
{
  public:
    glm::vec3 location;
    glm::vec3 rotation;
    glm::vec3 scale;
};
}