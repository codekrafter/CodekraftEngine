#pragma once

//#include "Display.hpp"
#include "Engine.hpp"

namespace ck
{
class GameApp;
class Display;
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