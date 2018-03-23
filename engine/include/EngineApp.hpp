#pragma once

#include "Display.hpp"
namespace ck
{
class CKEngine
{
private:
  Display *display;
  bool makeWindow;

public:
  CKEngine(bool md);
  ~CKEngine();
  Display *getDisplay();
  void update();
};
}