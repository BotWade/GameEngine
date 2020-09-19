#ifndef _COMPONENT_
#define _COMPONENT_

#include "Object.hpp"
#include "../Collisions/CollisionManager.hpp"

class Component {
public:
    Object* object;

    virtual void Load();
    virtual void Update();
    virtual void OnCollision(RayTestResult result);
    virtual void PreRender();
    virtual void Render();
    virtual void PosRender();
    virtual void Clean();
};

#endif