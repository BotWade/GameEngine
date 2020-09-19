#include "VulkanCommandPool.hpp"
#include "VulkanDeviceManager.hpp"
#include "../../Core/Debug.hpp"
#include "VulkanCommandBuffer.hpp"
#include "Vulkan.hpp"

void VulkanCommandPool::CreatePool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = VulkanDeviceManager::GetSelectedPhysicalDevice()->Indices.graphicsFamily.value();

    if (vkCreateCommandPool(VulkanDeviceManager::GetSelectedDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        Debug::Error("failed to create command pool!");
}

void VulkanCommandPool::Clean() {
    vkDestroyCommandPool(VulkanDeviceManager::GetSelectedDevice(), commandPool, nullptr);
}

void VulkanCommandPool::AddToQueuePool(VulkanCommandBuffer* commandBuffer) {
    while (QueueInUse.test_and_set()) { }
    drawQueue.push_back(commandBuffer->commandBuffers[Vulkan::imageIndex]);
    QueueInUse.clear();
}