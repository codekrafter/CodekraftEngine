#include "Material.hpp"
#include "AssetManager.hpp"
#include "Texture.hpp"
//#include "Display.hpp"
#include "Engine.hpp"
#include "WorldManager.hpp"

#include <string>
#include <ThirdParty/glm/glm.hpp>
#include <ThirdParty/glm/gtc/matrix_transform.hpp>
#include <ThirdParty/glm/gtc/type_ptr.hpp>

namespace ck
{
Material::Material()
{
    version = 1;
    type = "MATERIAL";
    LOG(INFO) << "Executig ck::Material()";
    //AssetManager *am = new AssetManager();
    //am->open("shaders.ckd");
    //shader = std::shared_ptr<Texture>(am->loadAsset<Shader>("phong"));
    //shader = new Shader();
    //shader->setCode(vv, fff);
    //am->reset();
    //delete am;
};
Material::Material(std::string d_name, std::string s_name, std::string prefix)
{
    version = 1;
    type = "MATERIAL";
    std::string dpath = prefix + d_name;
    std::string spath = prefix + s_name;
    diffuse = std::shared_ptr<Texture>(new Texture(dpath));
    if (s_name != "")
    {
        specular = std::shared_ptr<Texture>(new Texture(spath));
    }
    else
    {
        specular = std::shared_ptr<Texture>(new Texture(dpath, 2));
    }
    AssetManager *am = new AssetManager();

    am->open("shaders.ckd");
    empty_delete<Shader> ed;
    shader = am->loadAssetSPTR<Shader>("phong");
    //shader = std::shared_ptr<Shader>(am->loadAsset<Shader>("phong"), ed);
    //shader = std::shared_ptr<Shader>(new Shader());
    //shader->setCode(vv, fff);
    delete am;
    //am = nullptr;
};
Material::~Material(){
    //delete shader.get();
    //delete diffuse.get();
    //delete specular.get();

    //shader.reset();
    //diffuse.reset();
    //specular.reset();
};
template <class Archive>
void Material::serialize(Archive &ar)
{
    ar(cereal::base_class<ck::Asset>(this));
    ar(shader);
    ar(diffuse);
    ar(specular);
}

void Material::init()
{
    diffuse->init();
    specular->init();
    if (shader == nullptr)
    {
        LOG(ERROR) << "Shader in material is nullptr";
        return;
    }
    shader->init();
}

void Material::draw(Transform trans)
{
    shader->use();
    shader->setVec3("viewPos", WorldManager::getInstance()->getLevel()->getCamera()->Position);
    shader->setFloat("material.shininess", 32.0f);

    glm::mat4 projection = glm::perspective(glm::radians(WorldManager::getInstance()->getLevel()->getCamera()->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = WorldManager::getInstance()->getLevel()->getCamera()->GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    // world transformation
    glm::mat4 model;
    model = glm::translate(model, trans.location);

    model = glm::rotate(model, trans.rotation.x, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, trans.rotation.y, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, trans.rotation.z, glm::vec3(0.0, 0.0, 1.0));

    model = glm::scale(model, trans.scale);

    shader->setMat4("model", model);
    diffuse->draw(0);
    specular->draw(1);
}
}