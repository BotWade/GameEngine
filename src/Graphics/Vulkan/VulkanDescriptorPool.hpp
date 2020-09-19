#ifndef _VULKANDESCRIPTORPOOL_
#define _VULKANDESCRIPTORPOOL_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "Vulkan.hpp"

using namespace std;

class Shader;

class VulkanDescriptorPool {
public:
    VkDescriptorPool descriptorPool;
    
    void CreateDescriptorPool(Shader* shader, size_t FrameBufferSize = Vulkan::swapChainData.SwapChainImages.size());
    void Clean();
};

#endif