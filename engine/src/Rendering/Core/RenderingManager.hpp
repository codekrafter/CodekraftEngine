#pragma once

#include "Core/Config.hpp"
#include "Window.hpp"
#include "Renderer.hpp"

namespace ck
{

class RenderingManager
{
private:
  friend class CKEngine;
  RenderingManager(RenderingConfig dc);
  Window *window;
  Renderer *renderer;

public:
  static RenderingManager *inst();
  void preRender();
  void postRender();
  Renderer *getRenderer();
  Window *getWindow();
};

} // namespace ck