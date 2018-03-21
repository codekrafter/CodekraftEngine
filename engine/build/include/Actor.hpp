#pragma once

#include "GameObject.hpp"
#include "Transform.hpp"

namespace ck
{
class Actor : GameObject
{
  private:
  protected:
    Transform transform;

  public:
    inline Transform getTransform() { return transform; }
};
}