#pragma once

#define CKClass()
#define CKProperty()
#define CKEvent()

#include "ECS/SceneComponent.hpp"
#include "Assets/AssetManager.hpp"
#include "Rendering/StaticMesh.hpp"

namespace ck
{
CKClass();
class StaticMeshComponent : virtual public SceneComponent
{
private:
protected:
  AssetRef<StaticMesh> ref;
  friend class Editor;
  Material *mat;
  CKProperty();
  std::string test_string = "Test Value";

public:
  StaticMeshComponent() : ref("cube", "engine.meshes"){};
  StaticMeshComponent(AssetRef<StaticMesh> &r);
  virtual void render();
  virtual void onBeginPlay();
  CKEvent();
  void testEvent() { std::cout << "Test Event Triggered" << std::endl; };
};
} // namespace ck