#pragma once

#include "Rendering/Texture.hpp"
#include "Rendering/Shader.hpp"
#include "Assets/Asset.hpp"
#include "Assets/AssetManager.hpp"
#include "GLM.hpp"
#include "Engine.hpp"

namespace ckg
{
  struct MaterialS;
}

namespace ck
{
CKClass();
class Material : public Asset
{
private:
  Shader *shader = nullptr;

  //CKProperty();
  AssetRef<Texture> diffuse;

public:
  Shader *getShader();
  Material(AssetRef<Texture> diffuseIn);

private:
  friend struct ckg::MaterialS;
  Material();
};
} // namespace ck