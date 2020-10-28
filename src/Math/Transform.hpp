#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "Matrix4.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Quaternion.hpp"

class Object;

class Transform {
public:
    Object* object;

    Transform();

    Vector3 Up();
    Quaternion Rotation();
    Vector3 Scale();
    Vector3 Position();
    Matrix4 ModelMatrix();

    Quaternion localRotation;
    Vector3 localScale;
    Vector3 localPosition;
};

#endif