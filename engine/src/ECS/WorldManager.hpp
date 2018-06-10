#pragma once

#include <vector>
#include <string>

//#include "Level.hpp"

namespace ck
{
class Level;
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
  std::vector<std::string> nameReg;
};
}