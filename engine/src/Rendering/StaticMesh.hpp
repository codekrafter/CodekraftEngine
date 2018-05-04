#pragma once

#include <string>

#include "Assets/Asset.hpp"
#include "Mesh.hpp"
#include "ThirdParty/cereal/types/vector.hpp"

namespace ck
{
class StaticMesh : public Asset
{
private:
  friend struct StaticMeshS;
  std::vector<Mesh *> meshes;
  //std::vector<Material*> materials;

public:
  StaticMesh();
  StaticMesh(std::string path);
  ~StaticMesh();
  void loadRaw(std::string path);
  /*template <class Archive>
  void serialize(Archive &ar);*/
  virtual void init();
  void draw(Transform trans);
  std::vector<Mesh *> getMeshes();
  virtual std::string getType();
  virtual int getVersion();
};
}
// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
//#include <ThirdParty/cereal/types/polymorphic.hpp>
//#include <ThirdParty/cereal/archives/portable_binary.hpp>

//CEREAL_REGISTER_TYPE(ck::StaticMesh);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::StaticMesh)