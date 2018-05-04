#include "StaticMeshComponent.hpp"

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
  ref->draw(transform);
};
void StaticMeshComponent::onBeginPlay()
{
  ref->init();
};
}