#pragma once

#include "Actor.hpp"
#include "Model.hpp"

namespace ck
{
class StaticMeshActor : Actor
{
  private:
    Model *model;

  public:
    StaticMeshActor(std::string name);
    StaticMeshActor(Model *m);
    ~StaticMeshActor();
    virtual void onTick(float dt);
};
}