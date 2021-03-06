/*
#include <stdexcept>

#include "Rendering/DisplayOpenGL.hpp"
#include "DeferredShaderCode.hpp"
#include "ThirdParty/glad/glad.h"
#include "GLFW/glfw3.h"
#include "Mesh.hpp"
#include "Core/Engine.hpp"
#include "Core/EngineApp.hpp"
#include "ThirdParty/stb_image.h"
#include "GLM.hpp"
#include "Editor/Editor.hpp"
#include "ECS/WorldManager.hpp"
#include "Lighting/LightActor.hpp"
#include "ECS/Level.hpp"

#include "ThirdParty/OBJ_Loader.h"
#include "ThirdParty/IMGUI/imgui.h"
#include "ThirdParty/IMGUI/imgui_impl_glfw_gl3.h"
#include "ThirdParty/ImGuizmo.hpp"

namespace ck
{
namespace opengl
{
void DisplayOpenGL::glfw_error_callback(int error, const char *description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

DisplayOpenGL::DisplayOpenGL(RenderingConfig c) : Display(c)
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
    //glEnabled(GL_CULL);
    glfwHideWindow(window);

    shaderGeometryPass.setCode(geo_v, geo_f);
    shaderLightingPass.setCode(light_v, light_f);
    shaderLightingPass.init();

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    // - normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // create and attach depth buffer (renderbuffer)
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    // positions of the point lights
    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw mesh
    //shader->use();
    // Forward Render

    { // Deferred Block
        // 1. geometry pass: render scene's geometry/color data into gbuffer
        // -----------------------------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model;
        //shaderGeometryPass.use();
        //shaderGeometryPass.setMat4("projection", projection);
        //shaderGeometryPass.setMat4("view", view);
        WorldManager::getInstance()->getLevel()->tick(1.0f);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
        // -----------------------------------------------------------------------------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderLightingPass.use();
        shaderLightingPass.setInt("gPosition", 0);
        shaderLightingPass.setInt("gNormal", 1);
        shaderLightingPass.setInt("gAlbedoSpec", 2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

        // Basic Point Lights
        const unsigned int NR_LIGHTS = 32;
        std::vector<glm::vec3> lightPositions;
        std::vector<glm::vec3> lightColors;
        srand(13);

        for (Actor *a : WorldManager::getInstance()->getLevel()->getContents())
        {
            if (lightPositions.size() == 32)
            {
                break;
            }
            if (typeid(*a) == typeid(LightActor))
            {
                lightPositions.push_back(a->getTransform().location);
            }
        }

        // send light relevant uniforms
        for (unsigned int i = 0; i < lightPositions.size(); i++)
        {
            shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
            //shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
            shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Color", glm::vec3(1, 1, 1));

            // update attenuation parameters and calculate radius
            const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
            const float linear = 0.7;
            const float quadratic = 1.8;
            shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
            shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
            // then calculate radius of light volume/sphere
            //const float maxBrightness = std::fmaxf(std::fmaxf(1 lightColors[i].r, 1 /*lightColors[i].g*//*), /*lightColors[i].b*//* 1);
            float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
            shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
        }
        shaderLightingPass.setVec3("viewPos", camera.Position);
        // finally render quad
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderQuad();

        // 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
        // ----------------------------------------------------------------------------------
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
        // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the
        // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    } // Deferred Block

    ImGui::Render();
    ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void DisplayOpenGL::static_framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void DisplayOpenGL::static_mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    //Display *display = getEngine()->getDisplay();
    //DisplayOpenGL *d = dynamic_cast<DisplayOpenGL *>(display);
    //d->mouse_callback(window, xpos, ypos);
}

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

void DisplayOpenGL::renderQuad()
{
    if (quadVAO == 0)
    {
#define QUAD_V float quadVertices[] = { \
                   -1.0f,               \
                   1.0f,                \
                   0.0f,                \
                   0.0f,                \
                   1.0f,                \
                   -1.0f,               \
                   -1.0f,               \
                   0.0f,                \
                   0.0f,                \
                   0.0f,                \
                   1.0f,                \
                   1.0f,                \
                   0.0f,                \
                   1.0f,                \
                   1.0f,                \
                   1.0f,                \
                   -1.0f,               \
                   0.0f,                \
                   1.0f,                \
                   0.0f,                \
               };

        QUAD_V
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
} // namespace opengl
} // namespace ck
*/