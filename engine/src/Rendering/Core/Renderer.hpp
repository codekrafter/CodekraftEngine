#pragma once

#include "SceneInfo.hpp"
#include "GLM.hpp"

namespace ck
{
class StaticMesh;
class Material;
class Shader;
class Texture;
class Renderer
{
protected:
  SceneInfo sinfo;

  friend class RenderingManager;
  friend class CKEngine;
  virtual void init() = 0;
  virtual void preRender(SceneInfo info) = 0;
  virtual void postRender() = 0;

public:
  Renderer(){};

  virtual void init(StaticMesh *sm) = 0;
  virtual void init(Material *m) = 0;
  virtual void init(Shader *s) = 0;
  virtual void init(Texture *t) = 0;

  virtual void render(StaticMesh *sm, Material *m, glm::mat4 model) = 0;
};

} // namespace ck