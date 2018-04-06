#pragma once

<<<<<<< HEAD
#include "Display.hpp"
//#include "Engine.hpp"
#include "GameApp.hpp"
#include "Config.hpp"
=======
//#include "Display.hpp"
#include "Engine.hpp"
>>>>>>> 0cff946d6a15aadde52d8a42fbca0e1d803b90ba

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