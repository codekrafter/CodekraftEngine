#include "StaticMeshActor.hpp"

namespace ck
{
StaticMeshActor::StaticMeshActor(){};
StaticMeshActor::StaticMeshActor(std::string name){

};
StaticMeshActor::StaticMeshActor(StaticMesh *m)
{
    mesh = std::shared_ptr<StaticMesh>(m);
};
StaticMeshActor::~StaticMeshActor(){
    //mesh.reset();
};

void StaticMeshActor::onTick(float dt)
{
    mesh->draw();
};

template <class Archive>
void StaticMeshActor::serialize(Archive &archive)
{
    archive(/*cereal::base_class<ck::Asset>(this),*/ mesh);
};

void StaticMeshActor::onBeginPlay()
{
    mesh->init();
};
}