#ifndef _SHIPCOLLIDER_
#define _SHIPCOLLIDER_

#include "../Component.hpp"
#include "../../Collisions/Collider.hpp"
#include "../../Mesh/Mesh.hpp"

class ShipCollider : public Component {
public:
    Mesh* mesh;
    Collider* collider;

    void Load() override;
    void Render() override;
};

#endif