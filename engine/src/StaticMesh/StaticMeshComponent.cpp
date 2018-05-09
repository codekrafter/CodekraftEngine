#include "StaticMeshComponent.hpp"

#include "GLM.hpp"
#include "ThirdParty/ImGuizmo.hpp"
#include "ECS/Actor.hpp"

namespace ck
{
/*StaticMeshComponent::StaticMeshComponent()
{
  AssetRef<StaticMesh> r("cube", "engine.meshes");
  StaticMeshComponent(r);
};*/
StaticMeshComponent::StaticMeshComponent(AssetRef<StaticMesh> &r)
{
  ref = r;
};
void StaticMeshComponent::render()
{
  glm::mat4 model;
  Transform t;
  if (owner != nullptr)
  {
    t.location = transform.location + owner->getTransform().location;
    t.rotation = transform.rotation + owner->getTransform().rotation;
    t.scale = transform.scale * owner->getTransform().scale;
  }
  else
  {
    t = transform;
  }
  ImGuizmo::RecomposeMatrixFromComponents(&t.location.x, &t.rotation.x, &t.scale.x, &model[0][0]);

  ref->draw(model);
};
void StaticMeshComponent::onBeginPlay()
{
  ref->init();
};
} // namespace ck