#pragma once

#include <iostream>

#include "TP/glad/glad.h"
#include "GLFW/glfw3.h"
#include "Shader.hpp"
#include "StaticMesh.hpp"
#include <TP/glm/glm.hpp>
#include "TP/IMGUI/imgui.h"
#include "Camera.hpp"
#include <TP/glm/glm.hpp>
#include <TP/glm/gtc/matrix_transform.hpp>
#include <TP/glm/gtc/type_ptr.hpp>
#include "Level.hpp"

namespace ck
{
class Display
{
private:
  GLFWwindow *window;
  /// Is this a glfw display?
  bool glfw;

  // Callback
  void glfw_error_callback(int error, const char *description);
  static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void smouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
  void processInput(GLFWwindow *window);

  // settings
  static const unsigned int SCR_WIDTH = 800;
  static const unsigned int SCR_HEIGHT = 600;

  // camera
  Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  float lastX = SCR_WIDTH / 2.0f;
  float lastY = SCR_HEIGHT / 2.0f;
  bool firstMouse = true;

  // timing
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  // Used to render basic cube, not in final version
  unsigned int VBO, VAO, EBO;
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
  //std::vector<ck::Vertex> vertices;
  //std::vector<unsigned int> indices;

  bool show_demo_window = false;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

public:
  Display() : Display(false){};
  /// Use other display context (like in the editor)
  Display(bool econtext);
  /// Callbacks
  void mouse_callback(GLFWwindow *window, double xpos, double ypos);

  void update();
  ~Display();
  GLFWwindow *getWindow();
  ck::Shader *shader = nullptr;
  ck::StaticMesh *smesh = nullptr;
  Level *level = nullptr;
};
}