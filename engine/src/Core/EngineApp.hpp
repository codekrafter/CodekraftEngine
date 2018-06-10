#pragma once

#include "Rendering/Display.hpp"
//#include "Core/Engine.hpp"
#include "GameApp.hpp"
#include "Core/Config.hpp"

#include "Logging.hpp"

namespace ck
{
class GameApp;
class AssetManager;
class RenderingManager;
typedef std::chrono::time_point<std::chrono::high_resolution_clock> tunit;
class CKEngine
{
private:
  EngineConfig config;
  GameApp *game;
  friend class AssetManager;
  AssetManager *am;
  friend class RenderingManager;
  RenderingManager *rm;
  std::unique_ptr<g3::LogWorker> logworker{g3::LogWorker::createLogWorker()};

  tunit lastFrame;

public:
  CKEngine(EngineConfig c);
  ~CKEngine();
  void update();
  void init();
};
} // namespace ck