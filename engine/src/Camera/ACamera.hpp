#pragma once

#include "ECS/Actor.hpp"
#include "CameraUtils.hpp"

namespace ck
{

class ACamera : public Actor
{
  private:
    DirVectors dvectors;
    float yaw;
    float pitch;
    //void updateVectors();

  public:

};

} // namespace ck