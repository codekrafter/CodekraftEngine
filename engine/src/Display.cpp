#include <iostream>
#include <stdexcept>

#include "Display.hpp"
#include "TP/glad/glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <TP/stb_image.h>
#include <TP/glm/glm.hpp>
#include <TP/glm/gtc/matrix_transform.hpp>
#include <TP/glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "Engine.hpp"
#include "Editor.hpp"
#include "WorldManager.hpp"

#include "TP/OBJ_Loader.h"
#include "TP/IMGUI/imgui.h"
#include "TP/IMGUI/imgui_impl_glfw_gl3.h"
#include "TP/easylogging/easylogging++.h"
namespace ck
{
void Display::glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

Display::Display(bool econtext)
{
    if (!econtext)
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
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            throw std::runtime_error("Error: Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, smouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfw = true;

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
    }
    else
    {
        window = NULL;
        glfw = false;
    }
    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        throw std::runtime_error("Error: Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glfwHideWindow(window);
};

Display::~Display()
{
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
};

GLFWwindow *Display::getWindow()
{
    return window;
};

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Display::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        WorldManager::getInstance()->getLevel()->getCamera()->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
        editorKeyPressed = true;
    if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_RELEASE)
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

void Display::update()
{
    //std::cout << "started frame" << std::endl;
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
    // 1. Show a simple window.
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
    //ImGui::Image(ImTextureID(smesh->getMeshes()[0]->mat->diffuse->ID), ImVec2(smesh->getMeshes()[0]->mat->diffuse->width, smesh->getMeshes()[0]->mat->diffuse->height));
    //ImGui::Begin("Debug", &showDebug)
    //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    //ImGui::End();
    if (glfw)
    {
        if (Editor::getInstance()->showCursor())
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    Editor::getInstance()->Draw();

    // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
    if (show_demo_window)
    {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    // positions of the point lights
    /*glm::vec3 pointLightPositions[] = {
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
    shader->setVec3("pointLights[0].position", pointLightPositions[0]);
    shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[0].constant", 1.0f);
    shader->setFloat("pointLights[0].linear", 0.09);
    shader->setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    shader->setVec3("pointLights[1].position", pointLightPositions[1]);
    shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[1].constant", 1.0f);
    shader->setFloat("pointLights[1].linear", 0.09);
    shader->setFloat("pointLights[1].quadratic", 0.032);
    // point light 3
    shader->setVec3("pointLights[2].position", pointLightPositions[2]);
    shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    shader->setFloat("pointLights[2].constant", 1.0f);
    shader->setFloat("pointLights[2].linear", 0.09);
    shader->setFloat("pointLights[2].quadratic", 0.032);
    // point light 4
    shader->setVec3("pointLights[3].position", pointLightPositions[3]);
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

    //std::cout << "finished model drawing" << std::endl;

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
    //std::cout << "polled events" << std::endl;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Display::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void Display::smouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    getEngine()->getDisplay()->mouse_callback(window, xpos, ypos);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Display::mouse_callback(GLFWwindow *window, double xpos, double ypos)
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

    WorldManager::getInstance()->getLevel()->getCamera()->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Display::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
}
}