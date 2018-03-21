#pragma once

#include "Actor.hpp"
#include "StaticMesh.hpp"

namespace ck
{
class StaticMeshActor : Actor
{
private:
  StaticMesh *mesh;

public:
  StaticMeshActor(std::string name);
  StaticMeshActor(StaticMesh *m);
  ~StaticMeshActor();
  virtual void onTick(float dt);
};
}