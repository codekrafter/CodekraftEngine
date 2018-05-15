#pragma once

#include "Assets/Asset.hpp"
#include "ECS/Actor.hpp"

namespace ck
{
class ACamera;
class Level : public Asset
{
private:
  bool initialized = false;
  friend class Editor;
  friend class Display;
  friend class DisplayOpenGL;
  std::vector<Actor *> contents;
  //std::vector<Actor *> forward;

public:
  Level();
  virtual ~Level();
  virtual void init();
  void addActor(Actor *);
  void tick(float dt);
  inline std::vector<Actor *> getContents() { return contents; };
  ACamera *getCamera();
};
} // namespace ck

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
//#include <ThirdParty/cereal/types/polymorphic.hpp>
//#include <ThirdParty/cereal/archives/portable_binary.hpp>

//CEREAL_REGISTER_TYPE(ck::Level);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Level)