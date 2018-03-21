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

#include "TP/OBJ_Loader.h"

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
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfw = true;
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

    // Make Cube, not going to be in final engine version
    /*float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        };*/

    ///glGenVertexArrays(1, &VAO);
    ///glGenBuffers(1, &VBO);

    ///glBindVertexArray(VAO);

    ///glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    ///glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    ///glEnableVertexAttribArray(0);
    // texture coord attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
    /*  Mesh Data  */

    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    bool loaded = Loader.LoadFile("raw/nanosuit/nanosuit.obj");

    // Check to see if it loaded

    // If so continue
    if (!loaded)
    {
        throw std::runtime_error("Could not load .obj file");
        return;
    };

    for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
    {
        // Copy one of the loaded meshes to be our current mesh
        objl::Mesh curMesh = Loader.LoadedMeshes[i];

        // Print Mesh Name
        //indices.insert(std::end(indices), std::begin(curMesh.Indices), std::end(curMesh.Indices));
        std::cout << "Mesh " << i << ": " << curMesh.MeshName << std::endl;
        for (int j = 0; j < curMesh.Vertices.size(); j++)
        {
            ck::Vertex v;
            v.Position = glm::vec3(curMesh.Vertices[j].Position.X, curMesh.Vertices[j].Position.Y, curMesh.Vertices[j].Position.Z);
            v.Normal = glm::vec3(curMesh.Vertices[j].Normal.X, curMesh.Vertices[j].Normal.Y, curMesh.Vertices[j].Normal.Z);
            v.TexCoords = glm::vec2(curMesh.Vertices[j].TextureCoordinate.X, curMesh.Vertices[j].TextureCoordinate.Y);
            vertices.push_back(v);
        }

        for (int j = 0; j < curMesh.Indices.size(); j += 3)
        {
            indices.push_back(curMesh.Indices[j]);
            indices.push_back(curMesh.Indices[j + 1]);
            indices.push_back(curMesh.Indices[j + 2]);
        }
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ck::Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ck::Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ck::Vertex), (void *)offsetof(ck::Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ck::Vertex), (void *)offsetof(ck::Vertex, TexCoords));

    glBindVertexArray(0);
};

Display::~Display()
{
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
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.5f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.5f));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        view = glm::translate(view, glm::vec3(0.5f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        view = glm::translate(view, glm::vec3(-0.5f, 0.0f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        view = glm::translate(view, glm::vec3(0.0f, 0.5f, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, 0.0f));
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

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->use();

    // create transformations
    //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
    //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -.3f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader->ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader->setMat4("projection", projection);
    //std::cout << "set uniforms" << std::endl;

    // render box
    //glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    try
    {
        /*if (mmodel == nullptr)
        {
            std::cout << "mmodel is null" << std::endl;
            return;
        }*/
        //mmodel->Draw(shader);
    }
    catch (std::exception)
    {
        std::cerr << "caught exception" << std::endl;
    };
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

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void Display::mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    /*if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);*/
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Display::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(yoffset);
}