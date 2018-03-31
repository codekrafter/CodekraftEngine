#pragma once

#include "Display.hpp"

#include "ThirdParty/glad/glad.h"
#include "GLFW/glfw3.h"
#include <ThirdParty/glm/glm.hpp>

namespace ck
{
namespace opengl
{
class DisplayOpenGL : public Display
{
public:
  DisplayOpenGL(DisplayConfig c);
  virtual ~DisplayOpenGL();

private:
  GLFWwindow *window;
  float lastFrame = 0.0f;
  bool editorKeyPressed;
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;
  Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

  // Callbacks
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  void glfw_error_callback(int error, const char *description);
  static void static_framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void static_mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void static_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
  void processInput(GLFWwindow *window);

public:
};
}
}