#include "CommandBufferManager.hpp"
#include "VulkanDeviceManager.hpp"
#include "../../Math/Math.hpp"

VulkanCommandPool* CommandBufferManager::commandPool;
vector<VulkanCommandBuffer*> CommandBufferManager::commandBuffers;

void CommandBufferManager::RecreateCommandBuffers() {
    commandPool->CreatePool();
    for (VulkanCommandBuffer* commandBuffer : commandBuffers)
        commandBuffer->CreateCommandBuffer(commandPool);
}

VulkanCommandBuffer* CommandBufferManager::GetOrCreate(string Name, size_t CustomSize) {
    for (VulkanCommandBuffer* commandBuffer : commandBuffers)
        if (commandBuffer->Name == Name)
            return commandBuffer;

    VulkanCommandBuffer* commandBuffer = new VulkanCommandBuffer();

    if (CustomSize == __SIZE_MAX__)
        commandBuffer->CreateCommandBuffer(commandPool);
    else
        commandBuffer->CreateCommandBuffer(commandPool, CustomSize);

    commandBuffer->Name = Name;
    commandBuffers.push_back(commandBuffer);
    return commandBuffer;
}

VulkanCommandBuffer* CommandBufferManager::Create(size_t CustomSize) {
    VulkanCommandBuffer* commandBuffer = new VulkanCommandBuffer();

    if (CustomSize == __SIZE_MAX__)
        commandBuffer->CreateCommandBuffer(commandPool);
    else
        commandBuffer->CreateCommandBuffer(commandPool, CustomSize);

    bool FindingName = true;

    while (FindingName) {
        
        commandBuffer->Name = GenerateRandomString(20);
        
        FindingName = false;
        
        ///Verify The Name Wasn't Been Taken
        for (VulkanCommandBuffer* cb : commandBuffers)
            if (cb->Name == commandBuffer->Name)
                FindingName = true;
    }

    commandBuffers.push_back(commandBuffer);
    return commandBuffer;
}

void CommandBufferManager::Clear() {
    for (VulkanCommandBuffer* commandBuffer : commandBuffers)
        vkFreeCommandBuffers(VulkanDeviceManager::GetSelectedDevice(), commandPool->commandPool, static_cast<uint32_t>(commandBuffer->commandBuffers.size()), commandBuffer->commandBuffers.data());

    commandPool->Clean();
}