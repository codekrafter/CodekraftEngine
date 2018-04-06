#pragma once

#include "AssetManager.hpp"
#include "Shader.hpp"

namespace ck
{

class ResourceManager
{
private:
  virtual void abstract() = 0;
  static Shader *phong;

public:
  static Shader *getPhong();
  static void deleteResources();
};
}