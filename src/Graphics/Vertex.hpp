#ifndef _VERTEX_
#define _VERTEX_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

#include "../Math/Vector3.hpp"
#include "../Math/Vector2.hpp"

using namespace std;

struct Vertex {

    Vector3 Position;
    Vector3 Normal;
    Vector2 TextureCoordinate;
    
    static vector<VkVertexInputBindingDescription>* GetBindingDescription();
    static vector<VkVertexInputAttributeDescription>* GetAttributeDescriptions();
};

#endif