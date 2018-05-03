#include "StaticMeshActor.hpp"
#include "StaticMeshComponent.hpp"

namespace ck
{
StaticMeshActor::StaticMeshActor()
{
    components.push_back(new StaticMeshComponent());
};

StaticMeshActor::StaticMeshActor(AssetRef<StaticMesh> &r)
{
    components.push_back(new StaticMeshComponent(r));
};
}