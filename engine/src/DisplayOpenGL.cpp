#include <stdexcept>

#include "DisplayOpenGL.hpp"
#include "ThirdParty/glad/glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.hpp"
#include "Engine.hpp"
#include "EngineApp.hpp"
#include <ThirdParty/stb_image.h>
#include <ThirdParty/glm/glm.hpp>
#include <ThirdParty/glm/gtc/matrix_transform.hpp>
#include <ThirdParty/glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "Editor.hpp"
#include "WorldManager.hpp"

#include "ThirdParty/OBJ_Loader.h"
#include "ThirdParty/IMGUI/imgui.h"
#include "ThirdParty/IMGUI/imgui_impl_glfw_gl3.h"
#include "ThirdParty/easylogging/easylogging++.h"
namespace ck
{
namespace opengl
{
void DisplayOpenGL::glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

DisplayOpenGL::DisplayOpenGL(DisplayConfig c) : Display(c)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
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

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    ImGui_ImplGlfwGL3_Init(window, true);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG(FATAL) << "Failed to initialize GLAD";
        throw std::exception();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glfwHideWindow(window);
};

DisplayOpenGL::~DisplayOpenGL()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
};

bool DisplayOpenGL::shouldClose()
{
    return glfwWindowShouldClose(window);
};

void DisplayOpenGL::showWindow()
{
    glfwShowWindow(window);
};

void DisplayOpenGL::hideWindow()
{
    glfwHideWindow(window);
};
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void DisplayOpenGL::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
        editorKeyPressed = true;
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
    {
        if (editorKeyPressed)
        {
            Editor::getInstance()->toggleEditor();
            editorKeyPressed = false;
        }
        else
        {
            editorKeyPressed = false;
        }
    }
    if (Editor::getInstance()->showCursor())
        return;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(RIGHT, deltaTime);
}

void DisplayOpenGL::update()
{
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);
    ImGui_ImplGlfwGL3_NewFrame();

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

    // positions of the point lights
    /*glm::vec3 pointLighThirdPartyositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)};
    shader->use();
    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
    */
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //shader->use();
    /*
    // directional light
    shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    shader->setVec3("pointLights[0].position", pointLighThirdPartyositions[0]);
    shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[0].constant", 1.0f);
    shader->setFloat("pointLights[0].linear", 0.09);
    shader->setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    shader->setVec3("pointLights[1].position", pointLighThirdPartyositions[1]);
    shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[1].constant", 1.0f);
    shader->setFloat("pointLights[1].linear", 0.09);
    shader->setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    shader->setVec3("pointLights[2].position", pointLighThirdPartyositions[2]);
    shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[2].constant", 1.0f);
    shader->setFloat("pointLights[2].linear", 0.09);
    shader->setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    shader->setVec3("pointLights[3].position", pointLighThirdPartyositions[3]);
    shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[3].constant", 1.0f);
    shader->setFloat("pointLights[3].linear", 0.09);
    shader->setFloat("pointLights[3].quadratic", 0.032);
    // spotLight
    shader->setVec3("spotLight.position", camera.Position);
    shader->setVec3("spotLight.direction", camera.Front);
    shader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("spotLight.constant", 1.0f);
    shader->setFloat("spotLight.linear", 0.09);
    shader->setFloat("spotLight.quadratic", 0.032);
    shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    */ /*
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(WorldManager::getInstance()->getLevel()->getCamera()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = WorldManager::getInstance()->getLevel()->getCamera()->GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    // world transformation
    glm::mat4 model;
    //model = glm::translate(model, glm::vec3(0.0, 0.0, 0.3 * currentFrame));
    shader->setMat4("model", model);*/

    // draw mesh
    //shader->use();
    /*if (smesh != nullptr)
    {
        smesh->draw();
    }
    else
    {
        LOG(INFO) << "smesh null";
    }*/
    if (WorldManager::getInstance()->getLevel() != nullptr)
    {
        WorldManager::getInstance()->getLevel()->tick(1.0f);
    }
    else
    {
        LOG(INFO) << "level null";
    }
    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void DisplayOpenGL::static_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void DisplayOpenGL::static_mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    Display *display = getEngine()->getDisplay();
    DisplayOpenGL *d = dynamic_cast<DisplayOpenGL *>(display);
    d->mouse_callback(window, xpos, ypos);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void DisplayOpenGL::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;
    if (Editor::getInstance()->showCursor())
        return;

    WorldManager::getInstance()->getLevel()->getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void DisplayOpenGL::static_scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
}
}
}