#pragma once

#include <map>

#include "Rendering/Core/Renderer.hpp"
#include "Rendering/Shader.hpp"

namespace ck
{
struct MHandle
{
  unsigned int VBO, VAO, EBO;
  size_t i_size;
};

struct SMHandle
{
  std::vector<MHandle> mhandles;
};

class Asset;
class OpenGLRenderer : public Renderer
{
private:
  friend class CKEngine;
  friend class RenderingManager;
  virtual void init() override;
  virtual void preRender(SceneInfo info) override;
  virtual void postRender() override;

  /*
   * Handle Maps
   */

  // Single Handles (Textures, Shaders)
  std::map<Asset *, unsigned int> handles;
  // Handles for StaticMeshs
  std::map<StaticMesh *, SMHandle> SMHandles;
  unsigned int quadVAO, quadVBO;

  Shader Sgeo;
  Shader Slight;
  unsigned int gBuffer;
  unsigned int gPosition, gNormal, gAlbedoSpec;
  unsigned int rboDepth;

public:
  virtual void init(StaticMesh *sm) override;
  virtual void init(Material *m) override;
  virtual void init(Shader *s) override;
  virtual void init(Texture *t) override;

  virtual void render(StaticMesh *sm, Material *m, glm::mat4 model) override;
}; // namespace ck
} // namespace ck