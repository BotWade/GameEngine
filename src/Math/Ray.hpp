#ifndef _RAY_
#define _RAY_

#include "Vector3.hpp"

struct Ray {
    Vector3 Origin;
    Vector3 Direction;
    bool StopFirst;

    Ray();
    Ray(Vector3 origin, Vector3 direction);

    Vector3 GetPoint(float Distance);
    bool operator==(const Ray& other) const;
    bool operator<(const Ray& other) const;
};

#endif