#pragma once

#include "Actor.hpp"
#include "StaticMesh.hpp"

namespace ck
{
class StaticMeshActor : public Actor
{
private:
  std::shared_ptr<StaticMesh> mesh;

public:
  StaticMeshActor();
  StaticMeshActor(std::string name);
  StaticMeshActor(StaticMesh *m);
  ~StaticMeshActor();
  virtual void onTick(float dt);
  template <class Archive>
  void serialize(Archive &archive);
  virtual void onBeginPlay();
};
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
//#include <ThirdParty/cereal/types/polymorphic.hpp>
//#include <ThirdParty/cereal/archives/portable_binary.hpp>

//CEREAL_REGISTER_TYPE(ck::StaticMeshActor);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::StaticMeshActor)