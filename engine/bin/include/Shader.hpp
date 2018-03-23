#pragma once

#include <TP/glm/glm.hpp>

#include "Asset.hpp"
namespace ck
{
class Shader : public Asset
{
private:
  std::string vertex;
  std::string fragment;
  std::string geometry;
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(unsigned int shader, std::string type);

public:
  unsigned int ID;
  Shader();
  ~Shader();
  void init();
  template <class Archive>
  void serialize(Archive &ar);
  virtual std::string getType();
  virtual int getVersion();
  void setCode(std::string v, std::string f, std::string g = "");

  // activate the shader
  // ------------------------------------------------------------------------
  void use();

  // utility uniform functionss
  // ------------------------------------------------------------------------
  void setBool(const std::string &name, bool value) const;

  // ------------------------------------------------------------------------
  void setInt(const std::string &name, int value) const;

  // ------------------------------------------------------------------------
  void setFloat(const std::string &name, float value) const;
  // ------------------------------------------------------------------------
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w);
  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat) const;
};
}

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <TP/cereal/archives/binary.hpp>

CEREAL_REGISTER_TYPE(ck::Shader);

// Note that there is no need to register the base class, only derived classes
//  However, since we did not use cereal::base_class, we need to clarify
//  the relationship (more on this later)
CEREAL_REGISTER_POLYMORPHIC_RELATION(ck::Asset, ck::Shader)