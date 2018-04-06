#pragma once

#include <iostream>

//#include "Shader.hpp"
//#include "StaticMesh.hpp"
//#include "ThirdParty/IMGUI/imgui.h"
//#include <ThirdParty/glm/gtc/matrix_transform.hpp>
//#include <ThirdParty/glm/gtc/type_ptr.hpp>
<<<<<<< HEAD
//#include "Level.hpp"
//#include "Engine.hpp"
#include "Camera.hpp"
#include "Config.hpp"
=======
#include "Level.hpp"
#include "Engine.hpp"
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba

namespace ck
{
class Display
{
protected:
  DisplayConfig config;

  Camera camera;
  // timing
  float deltaTime = 0.0f;

  bool editorKeyPressed;

public:
  Display() : Display(DisplayConfig(DisplayType::OPENGL)){};
  Display(DisplayConfig c) : config(c){};
<<<<<<< HEAD
  virtual ~Display(){};

  virtual void update() = 0;
  virtual bool shouldClose() = 0;
  virtual void showWindow() = 0;
  virtual void hideWindow() = 0;
=======
  virtual ~Display() = 0;

  void update();
  virtual bool shouldClose() = 0;
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba
};
}