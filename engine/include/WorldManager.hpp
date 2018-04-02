#pragma once

#include "Level.hpp"

namespace ck
{
class WorldManager
{
private:
  static WorldManager *inst;
  Level *level;

public:
  WorldManager();
  ~WorldManager();
  static WorldManager *getInstance();
  static void destroyInstance();
  Level *getLevel();
  void loadLevel(Level *l);
};
}