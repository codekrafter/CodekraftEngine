#pragma once

#include "Core/EngineApp.hpp"

namespace ck
{
class GameApp
{
protected:
public:
  GameApp();
  virtual ~GameApp() = 0;
  virtual void onInit() = 0;
  virtual void onShutdown() = 0;
};
}