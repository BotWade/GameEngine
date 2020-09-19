#include "VulkanOffScreenRenderer.hpp"
#include "Vulkan.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanDeviceManager.hpp"

void VulkanOffScreenRenderer::Init(size_t SizeOfFramebuffer, Vector2 Size, VulkanRenderPass* renderPass) {
    for (size_t Index = 0; Index < SizeOfFramebuffer; Index++) {
        image.push_back(VkImage());
        imageMemory.push_back(VkDeviceMemory());
        Vulkan::CreateImage(Size, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image[Index], imageMemory[Index]);
        imageViews.push_back(Vulkan::CreateImageView(image[Index], VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT));
    }

    VkFormat depthFormat = Vulkan::FindDepthFormat();
    Vulkan::CreateImage(Size, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = Vulkan::CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    vector<vector<VkImageView>> views;

    for (VkImageView image : imageViews)
        views.push_back( { image, depthImageView });

    frameBuffer.CreateFramebuffers(renderPass, views, Size);
}

void VulkanOffScreenRenderer::Draw(vector<VkCommandBuffer> commandBuffer) {
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    VkSubmitInfo submitInfo {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffer.size());
	submitInfo.pCommandBuffers = commandBuffer.data();
	
    VkFenceCreateInfo fenceInfo {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = 0;

	VkFence fence;
    vkCreateFence(VulkanDeviceManager::GetSelectedDevice(), &fenceInfo, nullptr, &fence);
	vkQueueSubmit(VulkanDeviceManager::GetSelectedPhysicalDevice()->graphicsQueue, 1, &submitInfo, fence);
	vkWaitForFences(VulkanDeviceManager::GetSelectedDevice(), 1, &fence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(VulkanDeviceManager::GetSelectedDevice(), fence, nullptr);
}

void VulkanOffScreenRenderer::DrawAll(vector<vector<VkCommandBuffer>> commandBuffers) {
    for (size_t Index = 0; Index < imageViews.size(); Index++)
        Draw(commandBuffers[Index]);
}

void VulkanOffScreenRenderer::Clean() {
    frameBuffer.Clean();

    for (size_t Index = 0; Index < imageViews.size(); Index++) {
        vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), imageViews[Index], nullptr);
        vkDestroyImage(VulkanDeviceManager::GetSelectedDevice(), image[Index], nullptr);
        vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), imageMemory[Index], nullptr);
    }
    
    vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), depthImageView, nullptr);
    vkDestroyImage(VulkanDeviceManager::GetSelectedDevice(), depthImage, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), depthImageMemory, nullptr);
}