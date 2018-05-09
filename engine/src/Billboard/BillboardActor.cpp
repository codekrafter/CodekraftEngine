#include "BillboardActor.hpp"
#include "ECS/WorldManager.hpp"
#include "ECS/Level.hpp"

namespace ck
{

const std::string bb_v = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;//vec2(aTexCoord.x, aTexCoord.y);
}
)";

const std::string bb_f = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
	FragColor = texture(tex, TexCoord);
}
)";

void BillboardActor::onTick(float dt)
{
    // Rotate
    Camera *cam = WorldManager::getInstance()->getLevel()->getCamera();
    glm::mat4 mat = glm::lookAt(cam->Position, cam->Position + cam->Front, cam->Up);
    //transform.rotation = cam->Up + cam->Up;

    // Render
    shader->use();
    glm::mat4 projection = glm::perspective(glm::radians(WorldManager::getInstance()->getLevel()->getCamera()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = WorldManager::getInstance()->getLevel()->getCamera()->GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    glm::mat4 model = glm::lookAt(transform.location, cam->Position, cam->Up);
    /*model = glm::translate(model, transform.location);

    model = glm::rotate(model, transform.rotation.x, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, transform.rotation.y, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, transform.rotation.z, glm::vec3(0.0, 0.0, 1.0));

    model = glm::scale(model, transform.scale);*/

    shader->setMat4("model", model);
    icon->draw(0);

    glBindVertexArray(VAO);
    glPointSize(10);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
};

BillboardActor::BillboardActor()
{
    shader = AssetRef<Shader>("billboard", "engine.shaders");
    icon = AssetRef<Texture>("bbicon", "textures");
    /*if (icon->blank())
    {*/
    icon->loadRaw("raw/nanosuit/arm_dif.png");
    //}
};

void BillboardActor::onBeginPlay()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    shader->setCode(bb_v, bb_f);

    shader->init();
    icon->init();
}
} // namespace ck