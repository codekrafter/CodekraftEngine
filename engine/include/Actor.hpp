#pragma once

#include "GameObject.hpp"
#include "Transform.hpp"
#include "Asset.hpp"

namespace ck
{
class Actor : public GameObject//, public Asset
{
private:
protected:
  Transform transform;

public:
  Actor(){};
  //virtual ~Actor(){};
  inline Transform& getTransform() { return transform; }
};
}