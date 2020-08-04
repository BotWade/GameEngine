#include "Vector2.hpp"

Vector2::Vector2() {
    
    X = Y = 0.0f;
}
   
Vector2::Vector2(float x, float y) {
    
    X = x;
    Y = y;
}

bool Vector2::operator==(const Vector2& other) const {

    return X == other.X && Y == other.Y; 
}

bool Vector2::operator!=(const Vector2& other) const {

    return !(X == other.X && Y == other.Y);
}

Vector2 Vector2::operator+(const Vector2& right) const {

    return Vector2(X + right.X, Y + right.Y);
}

Vector2 Vector2::operator-(const Vector2& right) const {

    return Vector2(X - right.X, Y - right.Y);
}

Vector2 Vector2::operator/(const Vector2& right) const {

    return Vector2(X / right.X, Y / right.Y);
}

Vector2 Vector2::operator*(const Vector2& right) const {

    return Vector2(X * right.X, Y * right.Y);
}

Vector2 Vector2::operator*(const float& right) const {

    return Vector2(X * right, Y * right);
}