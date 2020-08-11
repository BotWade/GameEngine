#ifndef _PLAYERSHIP_
#define _PLAYERSHIP_

#include "Ship.hpp"

class PlayerShip : public Ship {
public:
    Mesh* movementMesh;

    void Load() override;
    void Update() override;
    void OnCollision(RayTestResult result) override;
    void PosRender() override;
};

#endif