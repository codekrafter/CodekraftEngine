#pragma once

#include "Display.hpp"
//#include "Engine.hpp"
#include "GameApp.hpp"
#include "Config.hpp"

namespace ck
{
class CKEngine
{
private:
  Display *display;
  EngineConfig config;
  GameApp *game;

public:
  CKEngine(EngineConfig c);
  ~CKEngine();
  Display *getDisplay();
  void update();
};
}