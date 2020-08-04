#ifndef _VECTOR3_
#define _VECTOR3_

#include "Vector2.hpp"

struct Vector3 {
    
    float X;
    float Y;
    float Z;

    Vector3();
    Vector3(float x, float y, float z);

    static Vector3 Zero();

    Vector2 Xy();

    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;

    Vector3 operator+(const Vector3& right) const;
    Vector3 operator-(const Vector3& right) const;
    Vector3 operator*(const Vector3& right) const;
    Vector3 operator*(const float& right) const;
    Vector3 operator/(const Vector3& right) const;
    Vector3 operator/(const float& right) const;

    Vector3& operator+=(const Vector3& right);
    Vector3& operator-=(const Vector3& right);

    float& operator[](int Index);

};



#endif