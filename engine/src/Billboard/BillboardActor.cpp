#include "BillboardActor.hpp"
#include "ECS/WorldManager.hpp"
#include "ECS/Level.hpp"
#include "ThirdParty/glad/glad.h"

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
    glm::mat4 model;
    model = glm::translate(model, transform.location);

    model = glm::rotate(model, transform.rotation.x, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, transform.rotation.y, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, transform.rotation.z, glm::vec3(0.0, 0.0, 1.0));

    model = glm::scale(model, transform.scale);
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

    //shader->init();
    //icon->init();
}
} // namespace ck