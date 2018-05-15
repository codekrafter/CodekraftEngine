#include "OpenGLRenderer.hpp"

extern "C"
{
#include "ThirdParty/glad/glad.h"
}

#include "Rendering/Core/SceneInfo.hpp"
#include "Rendering/StaticMesh.hpp"
#include "Rendering/Material/Material.hpp"
#include "Logging.hpp"
#include "ECS/WorldManager.hpp"
#include "ECS/Level.hpp"
#include "Lighting/LightActor.hpp"
#include "Rendering/DeferredShaderCode.hpp"
#include "Camera/ACamera.hpp"
#include "Camera/CameraUtils.hpp"

namespace ck
{
void OpenGLRenderer::init()
{
    if (quadVBO != 0 && quadVAO != 0)
    {
        return;
    }

    if ((quadVBO == 0 && quadVAO != 0) || (quadVBO != 0 && quadVAO == 0))
    {
        LOG(FATAL) << "Quad VBO and VAO are mixed up. One is zero, one is not. Aborting.";
    }
    if (!gladLoadGL())
    {
        LOG(FATAL) << "Failed to initialize GLAD";
        throw std::exception();
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    //glEnabled(GL_CULL);

    //SGeo.setCode(geo_v, geo_f);
    //Slight.setCode(light_v, light_f);
    //Slight.init();

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

    Slight.setCode(opengl::light_v, opengl::light_f);
    Slight.init();
};

void OpenGLRenderer::preRender(SceneInfo info)
{
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};

void OpenGLRenderer::postRender()
{

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glUseProgram(handles[m]);
    Slight.use();
    Slight.setInt("gPosition", 0);
    Slight.setInt("gNormal", 1);
    Slight.setInt("gAlbedoSpec", 2);
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
        Slight.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
        //Slight.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
        Slight.setVec3("lights[" + std::to_string(i) + "].Color", glm::vec3(1, 1, 1));

        // update attenuation parameters and calculate radius
        const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7;
        const float quadratic = 1.8;
        Slight.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
        Slight.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(1 /*lightColors[i].r*/, 1 /*lightColors[i].g*/), /*lightColors[i].b*/ 1);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        Slight.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
    }
    //Slight.setVec3("viewPos", WorldManager::getInstance()->getLevel()->getCamera()->Position);

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
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
};

void OpenGLRenderer::init(StaticMesh *sm)
{
    SMHandle smh;
    for (Mesh *m : sm->getMeshes())
    {
        unsigned int VBO, VAO, EBO;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        vertices = m->vertices;
        indices = m->indices;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);

        MHandle mh;
        mh.VBO = VBO;
        mh.VAO = VAO;
        mh.EBO = EBO;
        mh.i_size = indices.size();
        smh.mhandles.push_back(mh);
    }
    SMHandles[sm] = smh;
};

void OpenGLRenderer::init(Material *m)
{
    init(m->getShader());
};

void checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            LOG(ERROR) << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                       << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LOG(ERROR) << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                       << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void OpenGLRenderer::init(Shader *s)
{
    s->init();
    return;
    std::string vertex = s->vertex;
    std::string fragment = s->fragment;
    std::string geometry = s->geometry;

    // 2. compile shaders
    unsigned int vid, fid;
    int success;
    char infoLog[512];
    const char *vs = vertex.c_str();
    const char *fs = fragment.c_str();
    const char *gs = geometry.c_str();
    // vertex shader
    vid = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vid, 1, &vs, NULL);
    glCompileShader(vid);
    checkCompileErrors(vid, "VERTEX");
    // fragment Shader
    fid = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fid, 1, &fs, NULL);
    glCompileShader(fid);
    checkCompileErrors(fid, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int gid;
    if (geometry != "")
    {
        gid = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gid, 1, &gs, NULL);
        glCompileShader(gid);
        checkCompileErrors(gid, "GEOMETRY");
    }
    // shader Program
    unsigned int ID;
    ID = glCreateProgram();
    glAttachShader(ID, vid);
    glAttachShader(ID, fid);
    if (geometry != "")
        glAttachShader(ID, gid);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vid);
    glDeleteShader(fid);
    if (geometry != "")
        glDeleteShader(gid);

    handles[s] = ID;
};

void OpenGLRenderer::init(Texture *t)
{
    LOG(WARNING) << "Textures are not yet implemented";
};

void setMat4(const std::string &name, const glm::mat4 &mat, const unsigned int ID)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void OpenGLRenderer::render(StaticMesh *sm, Material *m, glm::mat4 model)
{
    if (quadVBO == 0 || quadVAO == 0)
    {
        LOG(WARNING) << "Initializing Renderer in render loop, please initialize before hand";
        init();
    }

    // Check for uninitialized Material(It's Shader)
    if (m->getShader()->ID == 0)
    {
        LOG(WARNING) << "Initializing Material in render loop, please initialize before hand";
        init(m);
    }

    if (SMHandles.find(sm) == SMHandles.end())
    {
        LOG(WARNING) << "Initializing Static Mesh in render loop, please initialize before hand";
        init(sm);
    }

    // Activate Material
    unsigned int ID = m->getShader()->ID;
    glUseProgram(ID);

    RenderingConfig rc;
    glm::mat4 projection = getProjection(rc);
    Transform trans = WorldManager::getInstance()->getLevel()->getCamera()->getTransform();
    glm::vec3 pos = trans.location;
    DirVectors dv = getVectors(trans.rotation.y, trans.rotation.x);
    glm::mat4 view = getViewMatrix(pos, dv);
    setMat4("projection", projection, ID);
    setMat4("view", view, ID);
    /*shader->setMat4("projection", projection);
    shader->setMat4("view", view);*/

    //shader->setMat4("model", model);
    setMat4("model", model, ID);

    // Draw Static Mesh
    SMHandle smh = SMHandles[sm];
    for (MHandle mh : smh.mhandles)
    {
        glBindVertexArray(mh.VAO);
        glDrawElements(GL_TRIANGLES, mh.i_size, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};
}; // namespace ck