#pragma once

#include "Actor.hpp"
#include "AssetManager.hpp"
#include "StaticMesh.hpp"

namespace ck
{
class BillboardActor : public Actor
{
private:
  AssetRef<Shader> shader;
  unsigned int VBO, VAO, EBO;
  float vertices[20] = {
        // positions        // texture coords
         0.0f,  1.0f,  1.0f, 1.0f, 1.0f,
         0.0f, -1.0f,  1.0f, 1.0f, 0.0f,
         0.0f, -1.0f, -1.0f, 0.0f, 0.0f, //
         0.0f,  1.0f, -1.0f, 0.0f, 1.0f
    };
  unsigned int indices[6] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

public:
  BillboardActor();
  virtual void onTick(float dt);
  virtual void onBeginPlay();
  AssetRef<Texture> icon;
};
}