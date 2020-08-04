#ifndef _VECTOR4_
#define _VECTOR4_

#include "Vector3.hpp"

struct Vector4 {
    
    float X;
    float Y;
    float Z;
    float W;

    Vector4();
    Vector4(Vector2 Xy, float z, float w);
    Vector4(Vector3 Xyz);
    Vector4(Vector3 Xyz, float w);
    Vector4(float value);
    Vector4(float x, float y, float z);
    Vector4(float x, float y, float z, float w);

    Vector3 Xyz();
    Vector2 Xy();

    Vector4 operator+(const Vector4& right) const;
    Vector4 operator-(const Vector4& right) const;
    Vector4 operator*(const float& right) const;
    Vector4 operator*(const Vector4& right) const;
};


#endif