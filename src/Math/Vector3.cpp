#include "Vector3.hpp"
#include "Quaternion.hpp"

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

Vector3 Vector3::RotateAround(Vector3 Position, Vector3 Target, Vector3 Up, float Angle) {

    Quaternion Quat = Quaternion::FromAxisAngle(Up, Angle);
    Vector3 Dif = Position - Target;
    Dif = Quat * Dif;
    return Target + Dif;
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

Vector3 Vector3::operator+(const Vector2& right) const {

    return Vector3(X + right.X, Y + right.Y, Z);
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

Vector3& Vector3::operator+=(const Vector2& right) {
    
    X += right.X;
    Y += right.Y;
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

    return X;
}

vector<VkVertexInputBindingDescription>* Vector3::GetBindingDescription() {
    vector<VkVertexInputBindingDescription>* bindingDescription = new vector<VkVertexInputBindingDescription>(1);
    (*bindingDescription)[0].binding = 0;
    (*bindingDescription)[0].stride = sizeof(Vector3);
    (*bindingDescription)[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

vector<VkVertexInputAttributeDescription>* Vector3::GetAttributeDescriptions() {
    vector<VkVertexInputAttributeDescription>* attributeDescriptions = new vector<VkVertexInputAttributeDescription>(1);
    (*attributeDescriptions)[0].binding = 0;
    (*attributeDescriptions)[0].location = 0;
    (*attributeDescriptions)[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    (*attributeDescriptions)[0].offset = 0;

    return attributeDescriptions;
}