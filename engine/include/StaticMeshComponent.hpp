#pragma once

#include "SceneComponent.hpp"
#include "AssetManager.hpp"
#include "StaticMesh.hpp"

namespace ck
{
class StaticMeshComponent : virtual public SceneComponent
{
private:
protected:
  AssetRef<StaticMesh> ref;

public:
  StaticMeshComponent() : ref("cube", "engine.meshes"){};
  StaticMeshComponent(AssetRef<StaticMesh> &r);
  virtual void render();
  virtual void onBeginPlay();
};
}