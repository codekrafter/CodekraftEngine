#include "Material.hpp"
#include "AssetManager.hpp"
#include "Texture.hpp"
#include <string>

namespace ck
{
Material::Material()
{
    version = 1;
    type = "MATERIAL";
};
Material::Material(std::string d_name, std::string s_name, std::string prefix)
{
    version = 1;
    type = "MATERIAL";
    std::string dpath = prefix + d_name;
    std::string spath = prefix + s_name;
    //std::cout << "dpath: " << dpath << std::endl;
    //std::cout << "spath: " << spath << std::endl;
    diffuse = new Texture(dpath);
    if (s_name != "")
    {
        specular = new Texture(spath);
    }
    else
    {
        specular = new Texture();
    }
    AssetManager *am = new AssetManager();
    am->open("test.ckd");
    shader = am->loadAsset<Shader>("phong");
    am->reset();
    delete am;
    am = nullptr;
};
Material::~Material()
{
    delete shader;
    delete diffuse;
    delete specular;
};
template <class Archive>
void Material::serialize(Archive &ar)
{
    ar(*shader, *diffuse, *specular);
}

void Material::init()
{
    diffuse->init();
    specular->init();
}

void Material::draw()
{
    shader->use();
    diffuse->draw(0);
    specular->draw(1);
}
}