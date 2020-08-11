#ifndef _Ship_
#define _Ship_

#include "../SpaceObject.hpp"

class Ship : public SpaceObject {
public:

    Vector3 MovementTarget;
    float orbitDistance;
    float speed;
    float acceleration;
    bool isOrbiting;
    bool inOrbit;
    
    virtual void Load() override;
    virtual void Update() override;
    virtual void Render() override;
};

#endif