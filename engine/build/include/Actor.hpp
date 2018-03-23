#pragma once

#include "GameObject.hpp"
#include "Transform.hpp"

namespace ck
{
class Actor : public GameObject
{
private:
protected:
  Transform transform;

public:
  Actor(){};
  inline Transform getTransform() { return transform; }
};
}