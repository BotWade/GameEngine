#include "Renderer.hpp"
#include "../Core/Debug.hpp"
#include "../Core/Camera.hpp"
#include "Vulkan/Vulkan.hpp"
#include "Vulkan/VulkanDeviceManager.hpp"
#include "../Core/ShaderManager.hpp"
#include "Vulkan/CommandBufferManager.hpp"
#include "Vulkan/PipelineManager.hpp"
#include "Vulkan/RenderPassManager.hpp"
#include "Vulkan/CommandPoolManager.hpp"
#include <array>

using namespace std;

VulkanCommandBuffer* Renderer::ClearCommandBuffer;
atomic_flag Renderer::QueueInUse;

void Renderer::Clear() {
    bool NeedProcessing = (ClearCommandBuffer == nullptr || Vulkan::SwapChainRecreated);

    if (ClearCommandBuffer == nullptr)
        ClearCommandBuffer = CommandBufferManager::GetOrCreate("Clear");

    if (NeedProcessing) {
        Vulkan::commandBuffer = ClearCommandBuffer;
        for (size_t Index = 0; Index < ClearCommandBuffer->commandBuffers.size(); Index++) {
            Vulkan::CurrentCommandBuffer = Index;

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
                Debug::Error("failed to begin recording command buffer!");

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = RenderPassManager::GetRenderPass("Clear")->renderPass;
            renderPassInfo.framebuffer = Vulkan::GetCurrentFramebuffer();
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = Vulkan::swapChainData.Extent;


            array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(Vulkan::GetCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            Vector2 Size = Window::GetSize();

            VkViewport viewport = { 0.0f, 0.0f, Size.X, Size.Y, 0.0f, 1.0f};
	        vkCmdSetViewport(Vulkan::GetCurrentCommandBuffer(), 0, 1, &viewport);

	        VkRect2D scissor = { { 0, 0 }, { static_cast<uint32_t>(Size.X), static_cast<uint32_t>(Size.Y) }};
	        vkCmdSetScissor(Vulkan::GetCurrentCommandBuffer(), 0, 1, &scissor);

            vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

            if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
                Debug::Error("failed to record command buffer!");
        }
    }

    CommandBufferManager::AddToQueuePool(ClearCommandBuffer);
}

void Renderer::Prepare() {
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = RenderPassManager::GetRenderPass("Default")->renderPass;
    renderPassInfo.framebuffer = Vulkan::GetCurrentFramebuffer();
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = Vulkan::swapChainData.Extent;

    array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(Vulkan::GetCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    Vector2 Size = Window::GetSize();

    VkViewport viewport = { 0.0f, 0.0f, Size.X, Size.Y, 0.0f, 1.0f};
	vkCmdSetViewport(Vulkan::GetCurrentCommandBuffer(), 0, 1, &viewport);

	VkRect2D scissor = { { 0, 0 }, { static_cast<uint32_t>(Size.X), static_cast<uint32_t>(Size.Y) }};
	vkCmdSetScissor(Vulkan::GetCurrentCommandBuffer(), 0, 1, &scissor);
}

SingleTimeCommandData Renderer::BeginSingleTimeCommands() {
    SingleTimeCommandData Data = SingleTimeCommandData();
    Data.commandPool = CommandPoolManager::GetCommandPool();

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Data.commandPool->commandPool;
    allocInfo.commandBufferCount = 1;
    
    vkAllocateCommandBuffers(VulkanDeviceManager::GetSelectedDevice(), &allocInfo, &Data.commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(Data.commandBuffer, &beginInfo);

    return Data;
}

void Renderer::EndSingleTimeCommands(SingleTimeCommandData Data) {
    vkEndCommandBuffer(Data.commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &Data.commandBuffer;

    while (QueueInUse.test_and_set()) { }
    vkQueueSubmit(VulkanDeviceManager::GetSelectedGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(VulkanDeviceManager::GetSelectedGraphicsQueue());
    QueueInUse.clear();

    vkFreeCommandBuffers(VulkanDeviceManager::GetSelectedDevice(), Data.commandPool->commandPool, 1, &Data.commandBuffer);
    CommandPoolManager::FreeCommandPool(Data.commandPool);
}


void Renderer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    SingleTimeCommandData Data = BeginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(Data.commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    EndSingleTimeCommands(Data);
}

uint32_t Renderer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags propertyFlags) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(VulkanDeviceManager::GetSelectedVkPhysicalDevice(), &memProperties);
    
    for (uint32_t Index = 0; Index < memProperties.memoryTypeCount; Index++)
        if ((typeFilter & (1 << Index)) && (memProperties.memoryTypes[Index].propertyFlags & propertyFlags) == propertyFlags)
            return Index;

    Debug::Error("Failed To Find Suitable Memory Type!");
    return -1;
}

void Renderer::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usageFlags;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(VulkanDeviceManager::GetSelectedDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        Debug::Error("Failed To Create Buffer!");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(VulkanDeviceManager::GetSelectedDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, propertyFlags);

    if (vkAllocateMemory(VulkanDeviceManager::GetSelectedDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        Debug::Error("Failed To Allocate Buffer Memory!");

    vkBindBufferMemory(VulkanDeviceManager::GetSelectedDevice(), buffer, bufferMemory, 0);
}

void Renderer::RendererClear() {

}

void Renderer::SetScissor(Vector4 Region) {
    vkCmdSetScissor(Vulkan::GetCurrentCommandBuffer(), 0, 1, new VkRect2D { (int32_t)Region.X, (int32_t)Region.Y, (uint32_t)Region.Z, (uint32_t)Region.W});
}

void Renderer::SetScissorDefault() {
    Vector2 WinSize = Window::GetSize();
    vkCmdSetScissor(Vulkan::GetCurrentCommandBuffer(), 0, 1, new VkRect2D { 0, 0, (uint32_t)WinSize.X, (uint32_t)WinSize.Y});
}

void Renderer::SetDepthMask(bool Active) {
    Vulkan::vkCmdSetDepthTestEnable(Vulkan::GetCurrentCommandBuffer(), Active);
}