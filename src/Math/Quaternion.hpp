#ifndef _QUATERNION_
#define _QUATERNION_

#include "Matrix4.hpp"

struct Quaternion {
public:
    Vector4 Quat;

    Quaternion();
    Quaternion(Vector4 quat);

    Vector3 ToEuler();

    static Quaternion EulerToQuaternion(Vector3 Euler);
    static Quaternion lerp(Quaternion Left, Quaternion Right, float Time);
    static Matrix4 toMatrix4(Quaternion quat);

    Quaternion operator*(float right);
    Quaternion operator*(Quaternion right);
    Quaternion operator+(Quaternion right);
    Vector3 operator*(Vector3 right);
};

#endif