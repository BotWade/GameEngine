#include "CommandBufferManager.hpp"
#include "VulkanDeviceManager.hpp"

VulkanCommandPool* CommandBufferManager::commandPool;
vector<VulkanCommandBuffer*> CommandBufferManager::commandBuffers;

VulkanCommandBuffer* CommandBufferManager::AddCommandBuffer(string Name, size_t CustomSize) {
    VulkanCommandBuffer* commandBuffer = new VulkanCommandBuffer();

    if (CustomSize == __SIZE_MAX__)
        commandBuffer->CreateCommandBuffer(commandPool);
    else
        commandBuffer->CreateCommandBuffer(commandPool, CustomSize);

    commandBuffer->Name = Name;
    commandBuffers.push_back(commandBuffer);
    return commandBuffer;
}

void CommandBufferManager::RecreateCommandBuffers() {
    commandPool->CreatePool();
    for (VulkanCommandBuffer* commandBuffer : commandBuffers)
        commandBuffer->CreateCommandBuffer(commandPool);
}

VulkanCommandBuffer* CommandBufferManager::GetOrCreate(string Name, size_t CustomSize) {
    for (VulkanCommandBuffer* commandBuffer : commandBuffers)
        if (commandBuffer->Name == Name)
            return commandBuffer;

    return AddCommandBuffer(Name, CustomSize);
}

void CommandBufferManager::Clear() {
    for (VulkanCommandBuffer* commandBuffer : commandBuffers)
        vkFreeCommandBuffers(VulkanDeviceManager::GetSelectedDevice(), commandPool->commandPool, static_cast<uint32_t>(commandBuffer->commandBuffers.size()), commandBuffer->commandBuffers.data());

    commandPool->Clean();
}