#include "VulkanCommandBuffer.hpp"
#include "VulkanDeviceManager.hpp"
#include "Vulkan.hpp"
#include "../../Core/Debug.hpp"

void VulkanCommandBuffer::CreateCommandBuffer(VulkanCommandPool* commandPool) {
    CreateCommandBuffer(commandPool, Vulkan::swapChainData.SwapChainImageViews.size());
}

void VulkanCommandBuffer::CreateCommandBuffer(VulkanCommandPool* commandPool, size_t FramebufferSize) {
    commandBuffers.resize(FramebufferSize);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool->commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(VulkanDeviceManager::GetSelectedDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        Debug::Error("failed to allocate command buffers!");
}