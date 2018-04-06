#pragma once

#include "Asset.hpp"

//#include <glad/glad.h>
//#include <ThirdParty/glm/glm.hpp>

#include "Asset.hpp"
#include "Actor.hpp"
#include "Camera.hpp"

namespace ck
{
class Level : public Asset
{
private:
  Camera *cam;
  bool initialized = false;

public:
  std::vector<std::shared_ptr<Actor>> contents;
  Level();
  virtual ~Level();
  virtual void init();
  template <class Archive>
  void serialize(Archive &ar);
  void tick(float dt);
  Camera *getCamera();
  void setCamera(Camera *c);
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

CEREAL_REGISTER_TYPE(ck::Level);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Level)