#include "Engine.hpp"
#include "StaticMeshActor.hpp"
#include "WorldManager.hpp"
#include "AssetManager.hpp"
#include "AssetStructures.hpp"
#include "ThirdParty/easylogging/easylogging++.h"

int main(int argc, char *argv[])
{
    ck::EngineConfig ec = ck::EngineConfig();
    ck::CKEngine *engine = ck::initEngine(ec);
    //ck::StaticMesh *smesh = new ck::StaticMesh("raw/nanosuit/nanosuit.obj");
    ck::AssetRef<ck::StaticMesh> smr("nanosuit","meshes");
    smr->loadRaw("raw/nanosuit/nanosuit.obj");
    //ck::StaticMeshS sms(smesh);
    //ck::DatSize ds = sms.save();
    //ck::AssetManager::inst()->save();

    //ck::AssetManager::cleanup();
    //ck::AssetManager::inst()->load();

    //ck::StaticMeshS smss;
    //smss.load(ds.data, ds.size);
    //ck::StaticMesh *sm = smss.asset();
    ck::Level *level = new ck::Level();
    ck::StaticMeshActor *sma = new ck::StaticMeshActor(smr);
    sma->getTransform().scale = glm::vec3(0.1,0.1,0.1);
    level->contents.push_back(std::shared_ptr<ck::StaticMeshActor>(sma));
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