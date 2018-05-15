#pragma once

#include "Rendering/Core/Window.hpp"

class GLFWwindow;

namespace ck
{
class GLFWWindow : public Window
{
private:
  GLFWwindow *window;

  static void error_callback(int error, const char *description);
  static void static_framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void static_mouse_callback(GLFWwindow *window, double xpos, double ypos);
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void static_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
  void processInput(GLFWwindow *window);

  bool EditorKeyPressed;
  static const unsigned int SCR_WIDTH = 800;
  static const unsigned int SCR_HEIGHT = 600;
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;

public:
  GLFWWindow(DrawType t) : Window(t){};
  ~GLFWWindow();
  virtual void init() override;
  virtual void preRender() override;
  virtual void postRender() override;

  virtual void setVisibility(bool v) override;
  virtual void setMouse(bool v) override;
  virtual bool shouldClose() override;
};

} // namespace ck