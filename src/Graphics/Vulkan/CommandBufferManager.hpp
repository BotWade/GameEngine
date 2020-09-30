#ifndef _COMMANDBUFFERMANAGER_
#define _COMMANDBUFFERMANAGER_

#include <vector>

#include "VulkanCommandBuffer.hpp"
#include "Vulkan.hpp"

using namespace std;

/**
* A Class Used To Keep Track Of All Command Bufer
*/
class CommandBufferManager {
public:
    static VulkanCommandPool* commandPool;
    static vector<VulkanCommandBuffer*> commandBuffers;

    /**
    * Add A Comand Buffer To The Draw Queue
    * @param commandBuffer The Command Buffer To Be Rendererd
    */
    static void AddToQueuePool(VulkanCommandBuffer* commandBuffer);
    /**
    * Add A New Command Buffer
    * @param CommandBuffer The Command Buffer To Be Added
    */
    static void AddCommandBuffer(VulkanCommandBuffer* CommandBuffer);
    /**
    * Get Or Create A New Command Buffer
    * @param Name The Name Of The Command Buffer
    * @param CustomSize (Optional) The Size Of The Command Buffer, If Not Set By Default It Will Be As Big As The SwapChain
    * @return The Command Buffer
    */
    static VulkanCommandBuffer* GetOrCreate(string Name, size_t CustomSize = __SIZE_MAX__);
    /**
    * Create A New Command Buffer The Name Of The Command Buffer Will Be Unique
    * @param CustomSize (Optional) The Size Of The Command Buffer, If Not Set By Default It Will Be As Big As The SwapChain
    * @return The Command Buffer
    */
    static VulkanCommandBuffer* Create(size_t CustomSize = __SIZE_MAX__);
    /**
    * Recreate All Command Buffers
    */
    static void RecreateCommandBuffers();
    /**
    * Clean All Command Buffers
    */
    static void Clear();
};

inline void CommandBufferManager::AddToQueuePool(VulkanCommandBuffer* commandBuffer) {
    commandPool->AddToQueuePool(commandBuffer);
}

inline void CommandBufferManager::AddCommandBuffer(VulkanCommandBuffer* CommandBuffer) {
    commandBuffers.push_back(CommandBuffer);
}

#endif