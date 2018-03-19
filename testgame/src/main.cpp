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
    //am->saveAsset(shader, "shader.cka");
    ck::Asset *shader = am->loadAsset("shader.cka");
    std::cout << "test" << std::endl;
    std::cout << "is: " << typeid(*shader).name() << std::endl;
    std::cout << "test2" << std::endl;
    //shader->deserialize(std::string(d));
    //shader->init();
    //run(engine);
    return 0;
}