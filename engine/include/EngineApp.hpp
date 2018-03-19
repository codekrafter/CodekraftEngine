#pragma once

#include "Display.hpp"

class CKEngine
{
  private:
    Display *display;

  public:
    CKEngine();
    ~CKEngine();
    Display *getDisplay();
    void update();
};