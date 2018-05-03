#pragma once

#include "Transform.hpp"
#include "ActorComponent.hpp"

namespace ck
{
class SceneComponent : virtual public ActorComponent
{
  protected:
    Transform transform;

  public:
    inline Transform &getTransform() { return transform; };
    inline void setTransform(Transform &trans) { transform = trans; };
};
}