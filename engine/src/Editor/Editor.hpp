#pragma once

#include "Assets/AssetManager.hpp"

namespace ck
{
class Actor;
class Editor
{
private:
  static Editor *inst;
  bool showEditor = false;
  // Scene Editor Data, right now stores one bool, second for making a std::pair and is not used
  std::map<Actor *, std::pair<bool, bool>> map;

public:
  Editor();
  static Editor *getInstance();
  static void destroyInstance();
  void Draw();
  bool showCursor();
  void toggleEditor();
};
}