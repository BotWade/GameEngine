#ifndef _UNIFORM_
#define _UNIFORM_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

using namespace std;

enum UniformType {
    UNIFORM_BUFFER,
    UNIFORM_TEXTURE
};

struct Uniform {
    string Name;
    VkDescriptorSetLayoutBinding LayoutBinding;
    UniformType type;

    Uniform(string name, uint32_t Index);
};


#endif