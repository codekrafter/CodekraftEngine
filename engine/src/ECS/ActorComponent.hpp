#pragma once

#include "GameObject.hpp"

namespace ck
{
class Actor;
class ActorComponent : virtual public GameObject
{
  private:
    friend class Actor;
    void setOwner(Actor *newOwner)
    {
        owner = newOwner;
    };

  protected:
    Actor *owner = nullptr;
};
}