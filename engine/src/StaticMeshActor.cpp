#include "StaticMeshActor.hpp"

namespace ck
{
StaticMeshActor::StaticMeshActor(std::string name){

};
StaticMeshActor::StaticMeshActor(StaticMesh *m){
    mesh = m;
};
StaticMeshActor::~StaticMeshActor(){};
void StaticMeshActor::onTick(float dt){};
}