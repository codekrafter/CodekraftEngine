#include <iostream>
#include <typeinfo>
#include "Engine.hpp"

const char *d = R"(
!!VERTEX
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(aPos, 1.0);
};
!!FRAGMENT
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords);
}
)";

int main(int argc, char *argv[])
{
    CKEngine *engine = initEngine();
    ck::AssetManager *am = new ck::AssetManager();
    ck::Shader *shader = new ck::Shader();
    am->saveAsset(shader, "shader.ckb");
    ck::AssetFile file = am->loadAsset("shader.ckb");
    //ck::Shader* shader = static_cast<ck::Shader*>(file.asset.get());
    //shader->deserialize(std::string(d));
    shader->init();
    shader->use();
    engine->getDisplay()->shader = shader;
    run(engine);
    return 0;
}