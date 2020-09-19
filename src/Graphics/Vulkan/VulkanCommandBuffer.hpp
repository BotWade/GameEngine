#ifndef _VULKANCOMMANDBUFFER_
#define _VULKANCOMMANDBUFFER_

#include "VulkanCommandPool.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

using namespace std;

class VulkanCommandBuffer {
public:
    vector<VkCommandBuffer> commandBuffers;
    string Name;
    
    void CreateCommandBuffer(VulkanCommandPool* commandPool);
    void CreateCommandBuffer(VulkanCommandPool* commandPool, size_t FramebufferSize);
};

#endif