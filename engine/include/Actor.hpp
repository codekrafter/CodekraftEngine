#pragma once

#include "SceneComponent.hpp"
#include "Asset.hpp"

namespace ck
{
class Actor : virtual public GameObject
{
private:
protected:
  Transform transform;
  friend class Level;
  std::vector<ActorComponent *> components;

public:
  Actor(){};
  virtual ~Actor()
  {
    for (ActorComponent *ac : components)
    {
      if (ac == nullptr)
        return;
      delete ac;
      ac = nullptr;
    }
  };
  inline Transform &getTransform() { return transform; };
  inline void setTransform(Transform &trans) { transform = trans; };
};
}