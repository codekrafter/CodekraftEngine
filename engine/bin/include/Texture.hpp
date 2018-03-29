#pragma once

#include "Asset.hpp"

namespace ck
{
class Texture : public Asset
{
private:
  unsigned char *data;

public:
  unsigned int ID;
  int width, height, n;
  Texture();
  Texture(std::string fname) : Texture(fname, 0) {};
  // Create texture with specified number of components, 0 for max number in file.
  Texture(std::string fname, int nn);
  ~Texture();
  template <class Archive>
  void serialize(Archive &ar);
  virtual void init();
  void draw(int i);
};
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <TP/cereal/archives/binary.hpp>

CEREAL_REGISTER_TYPE(ck::Texture);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::virtual_base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Texture)