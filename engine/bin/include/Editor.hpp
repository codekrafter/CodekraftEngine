#pragma once

#include "AssetManager.hpp"

namespace ck
{
class Editor
{
private:
  static Editor *inst;
  bool assetMenu = false;
  bool assetLoader = false;
  bool assetSaver = false;
  AssetManager *am = nullptr;
  bool showEditor = false;
  char fn[128];
  std::vector<AssetFile> openEditors;

  // Data for Asset Editors
  // Shader
  char vt[1024 * 16] = "";
  char ft[1024 * 16] = "";
  char gt[1024 * 16] = "";

public:
  Editor();
  ~Editor();
  static Editor *getInstance();
  static void destroyInstance();
  void Draw();
  bool showCursor();
  void toggleEditor();
  void drawassetMenus();
};
}