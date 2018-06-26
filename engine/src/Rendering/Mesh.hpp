#pragma once

#include "GLM.hpp"
#include "Assets/Asset.hpp"
#include "ThirdParty/OBJ_Loader.h"
#include "ThirdParty/cereal/types/vector.hpp"
#include "Rendering/Material/Material.hpp"
namespace ckg
{
  struct MeshS;
}

namespace ck
{
struct Vertex
{
  // position
  glm::vec3 Position;
  // normal
  glm::vec3 Normal;
  // texCoords
  glm::vec2 TexCoords;
  // tangent
  glm::vec3 Tangent;
  // bitangent
  glm::vec3 Bitangent;
};

CKClass();
class Mesh : public Asset
{
private:
  friend struct ckg::MeshS;
  friend class Renderer;
  friend class OpenGLRenderer;
  CKProperty();
  unsigned int VBO;
  CKProperty();
  unsigned int VAO;
  CKProperty();
  unsigned int EBO;
  //glm::mat4 trans;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

public:
  Material *mat;
  Mesh();
  Mesh(objl::Mesh m, std::string directory);
  virtual ~Mesh();
  virtual void init();
  void draw(glm::mat4 model);
};
}