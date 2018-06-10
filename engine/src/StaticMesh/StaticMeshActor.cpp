#include "StaticMesh/StaticMeshActor.hpp"
#include "StaticMeshComponent.hpp"

namespace ck
{
StaticMeshActor::StaticMeshActor()
{
    StaticMeshComponent comp;
    components.push_back(new StaticMeshComponent());
};

StaticMeshActor::StaticMeshActor(AssetRef<StaticMesh> &r)
{
    components.push_back(new StaticMeshComponent(r));
};
}