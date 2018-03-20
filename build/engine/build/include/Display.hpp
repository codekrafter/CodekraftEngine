#pragma once

#include <iostream>

#include "TP/glad/glad.h"
#include "GLFW/glfw3.h"

class Display
{
  private:
    GLFWwindow *window;
    /// Is this a glfw display?
    bool glfw;

    // Callback
    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    static void processInput(GLFWwindow *window);

    // settings
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

  public:
    Display() : Display(false) {};
    /// Use other display context (like in the editor)
    Display(bool econtext);
    void update();
    ~Display();
    GLFWwindow *getWindow();
};