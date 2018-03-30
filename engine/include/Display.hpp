#pragma once

#include <iostream>

//#include "Shader.hpp"
//#include "StaticMesh.hpp"
//#include "ThirdParty/IMGUI/imgui.h"
//#include "Camera.hpp"
//#include <ThirdParty/glm/gtc/matrix_transform.hpp>
//#include <ThirdParty/glm/gtc/type_ptr.hpp>
#include "Level.hpp"
#include "Engine.hpp"

namespace ck
{
class Display
{
protected:
  DisplayType config;

  Camera camera;
  // timing
  float deltaTime = 0.0f;

  bool editorKeyPressed;

public:
  Display();
  Display(DisplayType c);

  void update();
  virtual ~Display() = 0;
  virtual bool shouldClose() = 0;
};
}