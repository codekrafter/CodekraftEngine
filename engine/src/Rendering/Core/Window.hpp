#pragma once

namespace ck
{
enum class DrawType;
class Window
{
protected:
  DrawType dt;

public:
  Window(DrawType drawtype) { dt = drawtype; };
  virtual void init() = 0;
  virtual void preRender() = 0;
  virtual void postRender() = 0;

  virtual void setVisibility(bool v) = 0;
  virtual void setMouse(bool v) = 0;
  virtual bool shouldClose() = 0;
};

} // namespace ck