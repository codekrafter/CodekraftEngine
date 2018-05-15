#pragma once
/*
#include "Rendering/Display.hpp"
#include "Rendering/Shader.hpp"

//#include <ThirdParty/glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLM.hpp"

namespace ck
{
namespace opengl
{
class DisplayOpenGL : public Display
{
public:
  DisplayOpenGL(RenderingConfig c);
  virtual ~DisplayOpenGL();
  virtual bool shouldClose();
  virtual void update();
  virtual void showWindow();
  virtual void hideWindow();

private:
  GLFWwindow *window;
  float lastFrame = 0.0f;
  bool EditorKeyPressed;
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;
  Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

  // Deferred
  unsigned int gBuffer;
  unsigned int gPosition, gNormal, gAlbedoSpec;
  unsigned int rboDepth;

  Shader shaderGeometryPass;
  Shader shaderLightingPass;

  unsigned int quadVAO = 0;
  unsigned int quadVBO;
  void renderQuad();

  // Callbacks
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  void glfw_error_callback(int error, const char *description);
  static void static_framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void static_mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void static_scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
  void processInput(GLFWwindow *window);
};
}
}*/