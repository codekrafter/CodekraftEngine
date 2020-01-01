#include "StaticMeshComponent.hpp"

#include "Rendering/Texture.hpp"
#include "GLM.hpp"
#include "ThirdParty/ImGuizmo.hpp"
#include "ECS/Actor.hpp"
#include "Rendering/Core/RenderingManager.hpp"

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

  RenderingManager::inst()->getRenderer()->render(ref.get(), mat, model);

  //ref->draw(model);
};
void StaticMeshComponent::onBeginPlay()
{
  //ref->init();
  AssetRef<Texture> texture("texture", "textures");
  mat = new Material(texture);
  RenderingManager::inst()->getRenderer()->init(ref.get());
  RenderingManager::inst()->getRenderer()->init(mat);
  
};
} // namespace ck