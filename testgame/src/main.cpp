#include "Engine.hpp"
#include "StaticMeshActor.hpp"
#include "WorldManager.hpp"
#include "AssetManager.hpp"
#include "Serialization.hpp"
#include "DeferredShaderCode.hpp"
#include "Config.hpp"
#include "BillboardActor.hpp"

int main(int argc, char *argv[])
{
    ck::EngineConfig ec = ck::EngineConfig();
    ck::CKEngine *engine = ck::initEngine(ec);
    //ck::StaticMesh *smesh = new ck::StaticMesh("raw/nanosuit/nanosuit.obj");
    ck::AssetRef<ck::StaticMesh> nanosuit("nanosuit", "meshes");
    nanosuit->loadRaw("raw/nanosuit/nanosuit.obj");
    ck::AssetRef<ck::StaticMesh> plane("plane", "meshes");
    plane->loadRaw("raw/Plane.obj");
    plane->getMeshes()[0]->mat->shader->setCode(ck::opengl::geo_v, ck::opengl::white_f);
    //ck::StaticMeshS sms(smesh);
    //ck::DatSize ds = sms.save();
    //ck::AssetManager::inst()->save();

    //ck::AssetManager::cleanup();
    //ck::AssetManager::inst()->load();

    //ck::StaticMeshS smss;
    //smss.load(ds.data, ds.size);
    //ck::StaticMesh *sm = smss.asset();
    ck::Level *level = new ck::Level();
    ck::StaticMeshActor *nanosuit_actor = new ck::StaticMeshActor(nanosuit);
    nanosuit_actor->getTransform().scale = glm::vec3(0.1, 0.1, 0.1);
    ck::StaticMeshActor *plane_actor = new ck::StaticMeshActor(plane);
    //plane_actor->getTransform().scale = glm::vec3(0.1,0.1,0.1);
    plane_actor->getTransform().location = glm::vec3(1.5, 1.5, -.5);
    plane_actor->getTransform().rotation = glm::vec3(0, 90, 0);
    level->addActor(nanosuit_actor);
    level->addActor(plane_actor);
    //am->saveAsset("test-level", level);
    //am->close("test.ckd");

    //smesh->init();
    //shader->init();
    //level->init();
    //engine->getDisplay()->shader = shader;
    ck::WorldManager::getInstance()->loadLevel(level);
    run(engine);
    //delete shader;
    //delete model;
    return 0;
}