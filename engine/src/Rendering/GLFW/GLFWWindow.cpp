#include "GLFWWindow.hpp"

#include "GLFW/glfw3.h"
#include "GLM.hpp"
#include "Logging.hpp"
#include "ThirdParty/IMGUI/imgui.h"
#include "ThirdParty/IMGUI/imgui_impl_glfw_gl3.h"
#include "ThirdParty/ImGuizmo.hpp"
#include "Editor/Editor.hpp"
#include "ECS/WorldManager.hpp"
#include "ECS/Level.hpp"
#include "Rendering/Core/RenderingManager.hpp"
#include "Camera/ACamera.hpp"

namespace ck
{
void GLFWWindow::error_callback(int error, const char *description)
{
    LOGF(LOG_ERROR, "Error %d: %s\n", error, description);
};
void GLFWWindow::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Codekraft Engine", NULL, NULL);
    if (window == NULL)
    {
        LOG(FATAL) << "Failed to create GLFW window";
        glfwTerminate();
        throw std::exception();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, static_framebuffer_size_callback);
    glfwSetCursorPosCallback(window, static_mouse_callback);
    glfwSetScrollCallback(window, static_scroll_callback);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsDark();
};
void GLFWWindow::preRender()
{
    processInput(window);
    ImGui_ImplGlfwGL3_NewFrame();
    ImGuizmo::BeginFrame();

    bool showDebug = true;
    //ImGui::Begin("Debug", &showDebug)
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::End();
    if (Editor::getInstance()->showCursor())
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    Editor::getInstance()->Draw();
};
void GLFWWindow::postRender()
{
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
};

/*GLFWWindow::GLFWWindow(DrawType t)
{
    dt = t;
};*/

void GLFWWindow::processInput(GLFWwindow *window)
{
    float deltaTime = 1.0f;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        LOG(INFO) << "Closing";
    }
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
        EditorKeyPressed = true;
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
    {
        if (EditorKeyPressed)
        {
            Editor::getInstance()->toggleEditor();
            EditorKeyPressed = false;
        }
        else
        {
            EditorKeyPressed = false;
        }
    }
    if (Editor::getInstance()->showCursor())
        return;

    Transform trans = WorldManager::getInstance()->getLevel()->getCamera()->getTransform();
    DirVectors dv = getVectors(trans.rotation.y, trans.rotation.x);
    float moveSens = 0.01;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        //trans.location.x = trans.location.x + 0.1f;
        trans.location = trans.location + (dv.front * moveSens);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        trans.location = trans.location - (dv.front * moveSens);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        trans.location = trans.location - (dv.right * moveSens);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        trans.location = trans.location + (dv.right * moveSens);
    WorldManager::getInstance()->getLevel()->getCamera()->setTransform(trans);
}

GLFWWindow::~GLFWWindow()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
};

void GLFWWindow::setVisibility(bool v)
{
    if (v)
    {
        glfwShowWindow(window);
    }
    else
    {
        glfwHideWindow(window);
    }
};

void GLFWWindow::setMouse(bool v)
{
    if (v)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

bool GLFWWindow::shouldClose()
{
    return glfwWindowShouldClose(window);
}
void GLFWWindow::static_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GLFWWindow::static_mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    /*Display *display = getEngine()->getDisplay();
    DisplayOpenGL *d = dynamic_cast<DisplayOpenGL *>(display);*/
    GLFWWindow *w = dynamic_cast<GLFWWindow *>(RenderingManager::inst()->getWindow());
    w->mouse_callback(window, xpos, ypos);
}

void GLFWWindow::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = lastX - xpos;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    if (Editor::getInstance()->showCursor())
        return;

    //WorldManager::getInstance()->getLevel()->getCamera()->ProcessMouseMovement(xoffset, yoffset);

    xoffset *= 0.1;
    yoffset *= 0.4;

    Transform trans = WorldManager::getInstance()->getLevel()->getCamera()->getTransform();
    glm::vec3 r = trans.rotation;
    r.x += xoffset;
    r.y += yoffset;

    if (r.y > 89.0f)
        r.y = 89.0f;
    if (r.y < -89.0f)
        r.y = -89.0f;

    trans.rotation = r;
    WorldManager::getInstance()->getLevel()->getCamera()->setTransform(trans);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void GLFWWindow::static_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
}
} // namespace ck