#pragma once

#include "Rendering/Texture.hpp"
#include "Rendering/Shader.hpp"
#include "Assets/Asset.hpp"
#include "GLM.hpp"
#include "Engine.hpp"

namespace ck
{
CKClass();
class Material : public Asset
{
private:
  Shader *shader = nullptr;

public:
  Shader *getShader();
  Material();
};
} // namespace ck