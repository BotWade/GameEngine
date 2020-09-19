#include "VulkanFramebuffer.hpp"
#include "VulkanDeviceManager.hpp"
#include "Vulkan.hpp"
#include "../../Core/Debug.hpp"

void VulkanFramebuffer::CreateFramebuffers(VulkanRenderPass* renderPass) {
    size_t Size = Vulkan::swapChainData.SwapChainImageViews.size();
    Framebuffers.resize(Size);

    for (size_t Index = 0; Index < Size; Index++) {
        vector<VkImageView> attachments = {
            Vulkan::swapChainData.SwapChainImageViews[Index],
            Vulkan::depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = Vulkan::swapChainData.Extent.width;
        framebufferInfo.height = Vulkan::swapChainData.Extent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanDeviceManager::GetSelectedDevice(), &framebufferInfo, nullptr, &Framebuffers[Index]) != VK_SUCCESS)
            Debug::Error("failed to create framebuffer!");
    }    
}

void VulkanFramebuffer::CreateFramebuffers(VulkanRenderPass* renderPass, vector<vector<VkImageView>>& attachments, Vector2 Size) {
    Framebuffers.resize(attachments.size());

    for (size_t Index = 0; Index < attachments.size(); Index++) {
        
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass->renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments[Index].size());
        framebufferInfo.pAttachments = attachments[Index].data();
        framebufferInfo.width = Size.X;
        framebufferInfo.height = Size.Y;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(VulkanDeviceManager::GetSelectedDevice(), &framebufferInfo, nullptr, &Framebuffers[Index]) != VK_SUCCESS)
            Debug::Error("failed to create framebuffer!");
    }  
}

void VulkanFramebuffer::Clean() {
    for (auto framebuffer : Framebuffers)
        vkDestroyFramebuffer(VulkanDeviceManager::GetSelectedDevice(), framebuffer, nullptr);
}