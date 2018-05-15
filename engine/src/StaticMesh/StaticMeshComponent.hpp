#pragma once

#include "ECS/SceneComponent.hpp"
#include "Assets/AssetManager.hpp"
#include "Rendering/StaticMesh.hpp"

namespace ck
{
class StaticMeshComponent : virtual public SceneComponent
{
private:
protected:
  AssetRef<StaticMesh> ref;
  friend class Editor;
  Material *mat;

public:
  StaticMeshComponent() : ref("cube", "engine.meshes"){};
  StaticMeshComponent(AssetRef<StaticMesh> &r);
  virtual void render();
  virtual void onBeginPlay();
};
} // namespace ck