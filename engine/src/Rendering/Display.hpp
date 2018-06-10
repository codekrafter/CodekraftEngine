#pragma once

#include <iostream>

#include "Core/Config.hpp"

namespace ck
{

// settings
static const unsigned int SCR_WIDTH = 800;
static const unsigned int SCR_HEIGHT = 600;

class Display
{
protected:
  RenderingConfig config;

  // timing
  float deltaTime = 1.0f;

  bool EditorKeyPressed;

public:
  Display() : Display(RenderingConfig()){};
  Display(RenderingConfig c) : config(c){};
  virtual ~Display(){};

  virtual void update() = 0;
  virtual bool shouldClose() = 0;
  virtual void showWindow() = 0;
  virtual void hideWindow() = 0;
};
}