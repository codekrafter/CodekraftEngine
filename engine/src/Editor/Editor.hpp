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
  // Scene Editor Data, stores if open
  std::map<Actor *, bool> map;
  // Pointer to currently selected object
  Actor *selected = nullptr;
  int si = 0;

  // Shader Editor
  bool s_open;

public:
  Editor();
  static Editor *getInstance();
  static void destroyInstance();
  void Draw();
  bool showCursor();
  void toggleEditor();
};
}