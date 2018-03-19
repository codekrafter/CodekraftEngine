#pragma once

//#include <glad/glad.h>
#include <TP/glm/glm.hpp>

#include "Asset.hpp"
namespace ck
{
class Shader : public Asset
{
private:
  unsigned int ID;

  std::string *vertex = new std::string();
  std::string *fragment = new std::string();
  std::string *geometry = new std::string();
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(unsigned int shader, std::string type);

public:
  Shader();
  ~Shader();
  void init();
  virtual void deserialize(std::string data);
  virtual std::string serialize();
  virtual std::string getType();
  virtual int getVersion();

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