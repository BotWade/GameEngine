#ifndef _HIT_
#define _HIT_

#include "../Math/Vector3.hpp"

struct Collider;

struct Hit {
    
    Vector3 Origin;
    Vector3 Target;
    float Distance;
    Collider* Hitted;
    Vector3* Intersection;
};


#endif