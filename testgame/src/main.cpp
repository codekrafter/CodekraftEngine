#include <iostream>

#include "Core/Engine.hpp"
#include "StaticMesh/StaticMeshActor.hpp"
#include "ECS/WorldManager.hpp"
#include "Assets/AssetManager.hpp"
#include "Assets/Serialization.hpp"
#include "Rendering/DeferredShaderCode.hpp"
#include "Core/Config.hpp"
#include "Billboard/BillboardActor.hpp"
#include "ECS/Level.hpp"

#include "CK/Assets/TestAsset.hpp"
#include "CK/Logging.hpp"

int main(int argc, char *argv[])
{
    //std::cout << "starting" << std::endl;
    ck::EngineConfig ec = ck::EngineConfig();
    ck::CKEngine *engine = ck::initEngine(ec);
    //ck::StaticMesh *smesh = new ck::StaticMesh("raw/nanosuit/nanosuit.obj");
    //ck::AssetRef<ck::StaticMesh> nanosuit("nanosuit", "meshes");
    //nanosuit->loadRaw("raw/nanosuit/nanosuit.obj");
    ck::AssetRef<ck::TestAsset> test("test_asset");
    test->Ninit();
    ck::AssetManager::inst()->save();
    LOG(INFO) << "Saved Asset";
    //ck::AssetManager::inst()->load();
    //delete ck::AssetManager::inst();
    //ck::AssetManager::inst()->load();
    //ck::Level *level = new ck::Level();
    //ck::StaticMeshActor *nanosuit_actor = new ck::StaticMeshActor(nanosuit);
    //nanosuit_actor->getTransform().scale = glm::vec3(0.1, 0.1, 0.1);
    //level->addActor(nanosuit_actor);
    //ck::WorldManager::getInstance()->loadLevel(level);
    //run(engine);
    return 0;
}