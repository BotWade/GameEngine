#ifndef _VECTOR2_
#define _VECTOR2_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

struct Vector2 {
    
    float X, Y;

    Vector2();
    Vector2(float x, float y);

    bool operator==(const Vector2& other) const;
    bool operator!=(const Vector2& other) const;
    Vector2 operator+(const Vector2& right) const;
    Vector2 operator+(const float& right) const;
    Vector2 operator-(const Vector2& right) const;
    Vector2 operator-(const float& right) const;
    Vector2 operator*(const Vector2& right) const;
    Vector2 operator*(const float& right) const;
    Vector2 operator/(const Vector2& right) const;
    Vector2 operator/(const float& right) const;
    bool operator>(const Vector2& right) const;
    bool operator<(const Vector2& right) const;
    
    static vector<VkVertexInputBindingDescription>* GetBindingDescription();
    static vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
};


#endif