#pragma once

#include "Actor.hpp"
#include "StaticMesh.hpp"
#include "AssetManager.hpp"

namespace ck
{
class StaticMeshActor : public Actor
{

public:
  StaticMeshActor();
  StaticMeshActor(AssetRef<StaticMesh> &r);
};
}