#include "Rendering/Material/Material.hpp"
#include "Rendering/DeferredShaderCode.hpp"
#include "Rendering/Shader.hpp"

namespace ck
{

Material::Material()
{
    shader = new Shader();
    //shader->setCode(opengl::geo_v, opengl::white_f);
    shader->vfile = /*"raw/shaders/sm_v.glsl";*/"raw/shaders/basic_v.glsl";
    shader->ffile = /*"raw/shaders/sm_f.glsl";*/"raw/shaders/white_f.glsl";
    shader->open(shader->vfile, shader->ffile);
}

Material::Material(AssetRef<Texture> diffuseIn)
{
    shader = new Shader();
    //shader->setCode(opengl::geo_v, opengl::white_f);
    shader->vfile = /*"./raw/shaders/sm_v.glsl";*/"raw/shaders/basic_v.glsl";
    shader->ffile = /*"./raw/shaders/sm_f.glsl";*/"raw/shaders/white_f.glsl";
    shader->open(shader->vfile, shader->ffile);
    diffuse = diffuseIn;
}

Shader *Material::getShader()
{
    return shader;
}

} // namespace ck