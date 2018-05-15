#pragma once

#include "GLM.hpp"

#include "Assets/Asset.hpp"
namespace ck
{
class Shader : public Asset
{
private:
  friend class OpenGLRenderer;
  unsigned int ID;
  void checkCompileErrors(unsigned int shader, std::string type);

public:
  Shader();
  virtual void init();
  virtual std::string getType();
  virtual int getVersion();
  std::string vertex = "";
  std::string fragment = "";
  std::string geometry = "";
  std::string vfile;
  std::string ffile;
  void setCode(std::string v, std::string f, std::string g = "");
  void use();
  void destroy();
  void open(std::string vf, std::string ff);
  void reload();
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec2(const std::string &name, const glm::vec2 &value) const;
  void setVec2(const std::string &name, float x, float y) const;
  void setVec3(const std::string &name, const glm::vec3 &value) const;
  void setVec3(const std::string &name, float x, float y, float z) const;
  void setVec4(const std::string &name, const glm::vec4 &value) const;
  void setVec4(const std::string &name, float x, float y, float z, float w);
  void setMat2(const std::string &name, const glm::mat2 &mat) const;
  void setMat3(const std::string &name, const glm::mat3 &mat) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;
};
} // namespace ck