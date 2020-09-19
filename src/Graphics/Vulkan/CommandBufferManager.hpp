#ifndef _COMMANDBUFFERMANAGER_
#define _COMMANDBUFFERMANAGER_

#include <vector>

#include "VulkanCommandBuffer.hpp"

using namespace std;

class CommandBufferManager {
public:
    static VulkanCommandPool* commandPool;
    static vector<VulkanCommandBuffer*> commandBuffers;

    static void AddToQueuePool(VulkanCommandBuffer* commandBuffer);
    static VulkanCommandBuffer* AddCommandBuffer(string Name, size_t CustomSize = __SIZE_MAX__);
    static void AddCommandBuffer(VulkanCommandBuffer* CommandBuffer);
    static VulkanCommandBuffer* GetOrCreate(string Name, size_t CustomSize = __SIZE_MAX__);
    static void RecreateCommandBuffers();
    static void Clear();
};

inline void CommandBufferManager::AddToQueuePool(VulkanCommandBuffer* commandBuffer) {
    commandPool->AddToQueuePool(commandBuffer);
}

inline void CommandBufferManager::AddCommandBuffer(VulkanCommandBuffer* CommandBuffer) {
    commandBuffers.push_back(CommandBuffer);
}

#endif