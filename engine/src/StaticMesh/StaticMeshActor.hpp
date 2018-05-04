#pragma once

#include "ECS/Actor.hpp"
#include "Rendering/StaticMesh.hpp"
#include "Assets/AssetManager.hpp"

namespace ck
{
class StaticMeshActor : public Actor
{

public:
  StaticMeshActor();
  StaticMeshActor(AssetRef<StaticMesh> &r);
};
}