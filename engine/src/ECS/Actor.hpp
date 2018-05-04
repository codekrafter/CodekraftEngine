#pragma once

#include "ECS/SceneComponent.hpp"
#include "Assets/Asset.hpp"

namespace ck
{
class Actor : virtual public GameObject
{
private:
protected:
  Transform transform;
  friend class Level;
  std::vector<ActorComponent*> components;

public:
  Actor(){};
  virtual ~Actor(){};
  inline Transform &getTransform() { return transform; };
  inline void setTransform(Transform &trans) { transform = trans; };
};
}