#include "StaticMeshActor.hpp"

namespace ck
{
StaticMeshActor::StaticMeshActor(std::string name){

};
StaticMeshActor::StaticMeshActor(Model *m){
    model = m;
};
StaticMeshActor::~StaticMeshActor(){};
void StaticMeshActor::onTick(float dt){};
}