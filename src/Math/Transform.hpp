#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

class Transform {
public:
    Quaternion Rotation;
    Vector3 Scale;
    Vector3 Position;
};

#endif