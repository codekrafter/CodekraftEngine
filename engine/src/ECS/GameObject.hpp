#pragma once

namespace ck
{
class GameObject
{
private:
protected:
public:
  GameObject(){};
  virtual ~GameObject(){};
  // Do gameplay ticking, not rendering
  virtual void onTick(float dt){};
  // Before the deferred rendering, do setup tasks
  virtual void preRender(){};
  // During deferred rendering, draw objects
  virtual void render(){};
  // After deferred rendering, do cleanup or draw forward objects
  virtual void postRender(){};
  // Before level starts ticking, do setup
  virtual void onBeginPlay(){};
  // Before object is destroyed, do cleanup
  virtual void onDestroyed(){};
};
}