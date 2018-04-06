#pragma once

#include "Shader.hpp"
#include "Asset.hpp"
#include "Texture.hpp"
#include "Transform.hpp"

namespace ck
{
class Material : public Asset
{
private:
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture> specular;
  std::shared_ptr<Texture> diffuse;

public:
  Material();
  Material(std::string d_name, std::string s_name, std::string prefix = "");
  ~Material();
  template <class Archive>
  void serialize(Archive &ar);
  void draw(Transform trans);
  virtual void init();
};
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <ThirdParty/cereal/types/polymorphic.hpp>
<<<<<<< HEAD
#include <ThirdParty/cereal/archives/portable_binary.hpp>
=======
#include <ThirdParty/cereal/archives/binary.hpp>
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba

CEREAL_REGISTER_TYPE(ck::Material);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Material)