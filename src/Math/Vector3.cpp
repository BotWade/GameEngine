#include "Vector3.hpp"
#include <math.h>

Vector3::Vector3() {

    X = Y = Z = 0.0f;
}

Vector3::Vector3(float x, float y, float z) {

    X = x;
    Y = y;
    Z = z;
}

Vector3 Vector3::Zero() {
    return Vector3(0, 0, 0);
}

Vector2 Vector3::Xy() {
    return Vector2(X, Y);
}

float Vector3::Length() {
    return sqrt(X * X + Y * Y + Z * Z);
}

float Vector3::LengthSquared() {
    return X * X + Y * Y + Z * Z;
}

Vector3 Vector3::GetSafeNormal(float Tolerance) {
    float SquareSum = X*X + Y*Y + Z*Z;

    if (SquareSum == 1.0f)
        return *this;
    else if (SquareSum < Tolerance)
        return Vector3::Zero();
    
    float Scale = InvSqrt(SquareSum);
    return Vector3(X * Scale, Y * Scale, Z * Scale);
}

Vector3 Vector3::MoveTowards(Vector3 Current, Vector3 Target, float MaxDistanceDelta) {

    Vector3 Direction = Target - Current;
    float sqdist = Direction.X * Direction.X + Direction.Y * Direction.Y + Direction.Z * Direction.Z;

    if (sqdist == 0 || (MaxDistanceDelta >= 0 && sqdist <= MaxDistanceDelta * MaxDistanceDelta))
        return Target;

    float Dist = sqrt(sqdist);

    return Current + Direction / Dist * MaxDistanceDelta;
}

bool Vector3::operator==(const Vector3& other) const {

    return X == other.X && Y == other.Y && Z == other.Z;
}

bool Vector3::operator!=(const Vector3& other) const {

    return !(X == other.X && Y == other.Y && Z == other.Z);
}


Vector3 Vector3::operator+(const Vector3& right) const {

    return Vector3(X + right.X, Y + right.Y, Z + right.Z);
}

Vector3 Vector3::operator-(const Vector3& right) const {

    return Vector3(X - right.X, Y - right.Y, Z - right.Z);
}

Vector3 Vector3::operator*(const Vector3& right) const {

    return Vector3(X * right.X, Y * right.Y, Z * right.Z);
}

Vector3 Vector3::operator*(const float& right) const {

    return Vector3(X * right, Y * right, Z * right);
}

Vector3 Vector3::operator/(const Vector3& right) const {

    return Vector3(X / right.X, Y / right.Y, Z / right.Z);
}

Vector3 Vector3::operator/(const float& right) const {

    return Vector3(X / right, Y / right, Z / right);
}

Vector3& Vector3::operator+=(const Vector3& right) {
    
    X += right.X;
    Y += right.Y;
    Z += right.Z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& right) {

    X -= right.X;
    Y -= right.Y;
    Z -= right.Z;
    return *this;
}

float& Vector3::operator[](int Index) {

    switch (Index) {
        case 0:
                return X;
            break;
        case 1:
                return Y;
            break;
        case 2:
                return Z;
            break;
    }
}