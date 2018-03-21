#pragma once

namespace ck
{
class GameObject
{
  private:
  protected:
  public:
    GameObject();
    virtual ~GameObject();
    virtual void onTick(float dt);
    virtual void onBeginPlay();
    virtual void onDestroyed();
};
}