#pragma once

#include "Asset.hpp"
//#include <ThirdParty/cereal/access.hpp>

namespace ck
{
class Texture : public Asset
{
private:
  friend struct TextureS;
  unsigned char *data;
  unsigned int ID;
  int width, height, n;
  std::string path;

public:
  Texture();
  ~Texture();
  Texture(std::string fname) : Texture(fname, 0){};
  // Create texture with specified number of components, 0 for max number in file.
  Texture(std::string fname, int nn);
  void inline loadRaw(std::string fname) { loadRaw(fname, 0); };
  void loadRaw(std::string fname, int nn);
  inline bool blank() { return !data; };

  virtual void init();
  void draw(int i);
};
}

/*namespace cereal
{
template <class Archive>
struct specialize<Archive, ck::Texture, cereal::specialization::member_load_save>
{
};
}*/

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
//#include <ThirdParty/cereal/types/polymorphic.hpp>
//#include <ThirdParty/cereal/archives/portable_binary.hpp>

//CEREAL_REGISTER_TYPE(ck::Texture);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
//CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Texture)