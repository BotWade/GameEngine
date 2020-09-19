#ifndef _VULKANCOMMANDPOOL_
#define _VULKANCOMMANDPOOL_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <atomic>

using namespace std;

class VulkanCommandBuffer;

class VulkanCommandPool {
public:
    size_t UniqueID;
    VkCommandPool commandPool;
    vector<VkCommandBuffer> drawQueue;
    atomic_flag QueueInUse;

    void CreatePool();
    void Clean();
    void AddToQueuePool(VulkanCommandBuffer* commandBuffer); 
};

#endif