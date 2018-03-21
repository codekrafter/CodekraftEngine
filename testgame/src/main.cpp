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
    //am->open("test.ckd");
    ck::Shader *shader = new ck::Shader();
    am->saveAsset("white-shader", shader);
    //ck::AssetFile shaderFile = am->loadAsset("shader.ckb");
    //ck::Asset *sasset = shaderFile.asset.get();
    //std::cout << typeid(*sasset).name() << std::endl;
    //ck::Shader *shader = static_cast<ck::Shader *>(sasset);
    ck::StaticMesh *smesh = new ck::StaticMesh("raw/nanosuit/nanosuit.obj");
    am->saveAsset("nanosuit", smesh);
    //ck::AssetFile modelFile = am->loadAsset("nanosuit.ckb");
    //std::cout << "type: " << modelFile.type << std::endl;
    //ck::Asset *masset = modelFile.asset.get();
    //ck::StaticMesh *smesh = dynamic_cast<ck::StaticMesh *>(masset);
    std::cout << "# of meshes: " << smesh->getMeshes().size() << std::endl;
    am->close("test.ckd");

    smesh->init();
    shader->init();
    shader->use();
    engine->getDisplay()->shader = shader;
    engine->getDisplay()->smesh = smesh;
    run(engine);
    //delete shader;
    //delete model;
    return 0;
}