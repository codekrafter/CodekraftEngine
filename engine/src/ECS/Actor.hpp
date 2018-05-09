#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#include "ECS/SceneComponent.hpp"
#include "Assets/Asset.hpp"
#include "ECS/WorldManager.hpp"

namespace ck
{
class Actor : virtual public GameObject
{
private:
  std::string name = "";

protected:
  Transform transform;
  friend class Level;
  std::vector<ActorComponent *> components;

public:
  Actor(){};
  virtual ~Actor(){};
  inline Transform &getTransform() { return transform; };
  void setTransform(Transform &trans) { transform = trans; };
  virtual void onBeginPlay();
  /*virtual void render()
  {
    for (ActorComponent *ac : components)
    {
      ac->render();
    };
  }*/
  std::string getName();

  void setName(std::string n);
};
} // namespace ck

#endif // ACTOR_H