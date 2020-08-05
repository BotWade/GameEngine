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

Vector3 Vector3::MoveTowards(Vector3 Current, Vector3 Target, float MaxDistanceDelta) {

            float toVector_x = Target.X - Current.X;
            float toVector_y = Target.Y - Current.Y;
            float toVector_z = Target.Z - Current.Z;

            float sqdist = toVector_x * toVector_x + toVector_y * toVector_y + toVector_z * toVector_z;

            if (sqdist == 0 || (MaxDistanceDelta >= 0 && sqdist <= MaxDistanceDelta * MaxDistanceDelta))
                return Target;
            float dist = sqrt(sqdist);

            return Vector3(Current.X + toVector_x / dist * MaxDistanceDelta,
                Current.Y + toVector_y / dist * MaxDistanceDelta,
                Current.Z + toVector_z / dist * MaxDistanceDelta);
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