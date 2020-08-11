#ifndef _SPACEOBJECT_
#define _SPACEOBJECT_

#include "Component.hpp"
#include "../Mesh/Mesh.hpp"
#include "../Collisions/Collider.hpp"

#include <string>

using namespace std;

class SpaceObject : public Component {
public:
    Mesh* mesh;
    Shader* shader;
    Collider* collider;

    Matrix4 model;

    bool isOrbitable;

    SpaceObject();

    void LoadObject(const char* Shader, const char* Model);
    void LoadCollider(Mesh* Collider);
    virtual void Update() override;
    virtual void Render() override;
};

#endif