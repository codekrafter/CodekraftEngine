#pragma once

#include "Rendering/Shader.hpp"
#include "Assets/Asset.hpp"
#include "Texture.hpp"
#include "ECS/Transform.hpp"

namespace ck
{
class Material : public Asset
{
private:
  friend struct MaterialS;
  Texture *specular;
  Texture *diffuse;

public:
  Material();
  Material(std::string d_name, std::string s_name, std::string prefix = "");
  ~Material();
  void draw(glm::mat4 model);
  virtual void init();
  Shader *shader;
};
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
//#include <ThirdParty/cereal/types/polymorphic.hpp>
//#include <ThirdParty/cereal/archives/portable_binary.hpp>

//CEREAL_REGISTER_TYPE(ck::Material);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Material)