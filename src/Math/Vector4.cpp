#include "Vector4.hpp"

Vector4::Vector4() {

    X = Y = Z = W = 0;
}

Vector4::Vector4(Vector2 Xy, float z, float w) {

    X = Xy.X;
    Y = Xy.Y;
    Z = z;
    W = w;
}

Vector4::Vector4(Vector3 Xyz) {

    X = Xyz.X;
    Y = Xyz.Y;
    Z = Xyz.Z;
    W = 1;
}

Vector4::Vector4(Vector3 Xyz, float w) {

    X = Xyz.X;
    Y = Xyz.Y;
    Z = Xyz.Z;
    W = w;
}

Vector4::Vector4(float value) {

    X = Y = Z = W = value;
}

Vector4::Vector4(float x, float y, float z) {

    X = x;
    Y = y;
    Z = z;
    W = 1;
}

Vector4::Vector4(float x, float y, float z, float w) {

    X = x;
    Y = y;
    Z = z;
    W = w;
}

void Vector4::Xyz(Vector3 Xyz) {
    X = Xyz.X;
    Y = Xyz.Y;
    Z = Xyz.Z;
}

Vector3 Vector4::Xyz() {

    return Vector3(X, Y, Z);
}

Vector2 Vector4::Xy() {

    return Vector2(X, Y);
}

Vector4 Vector4::operator+(const Vector4& right) const {

    return Vector4(X + right.X, Y + right.Y, Z + right.Z, W + right.W);
}

Vector4 Vector4::operator-(const Vector4& right) const {

    return Vector4(X - right.X, Y - right.Y, Z - right.Z, W - right.W);
}

Vector4 Vector4::operator*(const float& right) const {

    return Vector4(X * right, Y * right, Z * right, W * right);
}

Vector4 Vector4::operator*(const Vector4& right) const {

    return Vector4(X * right.X, Y * right.Y, Z * right.Z, W * right.W);
}

Vector4& Vector4::operator+=(const Vector4& right) {
    
    X += right.X;
    Y += right.Y;
    Z += right.Z;
    W += right.W;
    return *this;
}

Vector4& Vector4::operator*=(const float& right) {
    
    X *= right;
    Y *= right;
    Z *= right;
    W *= right;
    return *this;
}