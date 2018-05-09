#pragma once

#include "Rendering/Display.hpp"
//#include "Core/Engine.hpp"
#include "GameApp.hpp"
#include "Core/Config.hpp"

#include "Logging.hpp"

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
  std::unique_ptr<g3::LogWorker> logworker{ g3::LogWorker::createLogWorker() };


public:
  CKEngine(EngineConfig c);
  ~CKEngine();
  Display *getDisplay();
  void update();
};
}