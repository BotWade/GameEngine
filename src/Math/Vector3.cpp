#include "Vector3.hpp"

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