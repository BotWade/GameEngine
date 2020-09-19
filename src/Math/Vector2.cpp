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

Vector2 Vector2::operator+(const float& right) const {

    return Vector2(X + right, Y + right);
}

Vector2 Vector2::operator-(const Vector2& right) const {

    return Vector2(X - right.X, Y - right.Y);
}

Vector2 Vector2::operator-(const float& right) const {

    return Vector2(X - right, Y - right);
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

Vector2 Vector2::operator/(const float& right) const {

    return Vector2(X / right, Y / right);
}

bool Vector2::operator>(const Vector2& right) const {
    return X > right.X && Y > right.Y;    
}

bool Vector2::operator<(const Vector2& right) const {
    return X < right.X && Y < right.Y;
}

vector<VkVertexInputBindingDescription>* Vector2::GetBindingDescription() {
    vector<VkVertexInputBindingDescription>* bindingDescription = new vector<VkVertexInputBindingDescription>(1);
    (*bindingDescription)[0].binding = 0;
    (*bindingDescription)[0].stride = sizeof(Vector2);
    (*bindingDescription)[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
}

vector<VkVertexInputAttributeDescription>* Vector2::GetAttributeDescriptions() {
    vector<VkVertexInputAttributeDescription>* attributeDescriptions = new vector<VkVertexInputAttributeDescription>(1);
    (*attributeDescriptions)[0].binding = 0;
    (*attributeDescriptions)[0].location = 0;
    (*attributeDescriptions)[0].format = VK_FORMAT_R32G32_SFLOAT;
    (*attributeDescriptions)[0].offset = 0;

    return attributeDescriptions;
}