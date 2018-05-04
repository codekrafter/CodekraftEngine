#pragma once

#include <iostream>

#include "Camera/Camera.hpp"
#include "Core/Config.hpp"

namespace ck
{

// settings
static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

class Display
{
protected:
  DisplayConfig config;

  Camera camera;
  // timing
  float deltaTime = 0.0f;

  bool EditorKeyPressed;

public:
  Display() : Display(DisplayConfig(DisplayType::OPENGL)){};
  Display(DisplayConfig c) : config(c){};
  virtual ~Display(){};

  virtual void update() = 0;
  virtual bool shouldClose() = 0;
  virtual void showWindow() = 0;
  virtual void hideWindow() = 0;
};
}