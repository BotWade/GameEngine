#include "VulkanRenderPass.hpp"
#include "VulkanDeviceManager.hpp"
#include "Vulkan.hpp"
#include "../../Core/Debug.hpp"

using namespace std;

void VulkanRenderPass::PrepareRenderPass() {
    renderPassData.ColorAttachment.format = Vulkan::swapChainData.SwapChainFortmat;
    renderPassData.ColorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    renderPassData.ColorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassData.ColorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassData.ColorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassData.ColorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassData.ColorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassData.ColorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    renderPassData.DepthAttachment.format = Vulkan::FindDepthFormat();
    renderPassData.DepthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    renderPassData.DepthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    renderPassData.DepthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    renderPassData.DepthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    renderPassData.DepthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    renderPassData.DepthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    renderPassData.DepthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    renderPassData.ColorAttachmentRef.attachment = 0;
    renderPassData.ColorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    renderPassData.DepthAttachmentRef.attachment = 1;
    renderPassData.DepthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    renderPassData.Subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    renderPassData.Subpass.colorAttachmentCount = 1;
    renderPassData.Subpass.pColorAttachments = &renderPassData.ColorAttachmentRef;
    renderPassData.Subpass.pDepthStencilAttachment = &renderPassData.DepthAttachmentRef;

    renderPassData.Dependency.push_back({ });
    renderPassData.Dependency.push_back({ });
    
    renderPassData.Dependency[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    renderPassData.Dependency[0].dstSubpass = 0;
    renderPassData.Dependency[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    renderPassData.Dependency[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    renderPassData.Dependency[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    renderPassData.Dependency[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    renderPassData.Dependency[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    renderPassData.Dependency[1].srcSubpass = 0;
    renderPassData.Dependency[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    renderPassData.Dependency[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    renderPassData.Dependency[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    renderPassData.Dependency[1].srcAccessMask =  VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    renderPassData.Dependency[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    renderPassData.Dependency[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    renderPassData.attachments = {renderPassData.ColorAttachment, renderPassData.DepthAttachment};
    renderPassData.RenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassData.RenderPassInfo.pNext = NULL;
    renderPassData.RenderPassInfo.attachmentCount = static_cast<uint32_t>(renderPassData.attachments.size());
    renderPassData.RenderPassInfo.pAttachments = renderPassData.attachments.data();
    renderPassData.RenderPassInfo.subpassCount = 1;
    renderPassData.RenderPassInfo.pSubpasses = &renderPassData.Subpass;
    renderPassData.RenderPassInfo.dependencyCount = static_cast<uint32_t>(renderPassData.Dependency.size());
    renderPassData.RenderPassInfo.pDependencies = renderPassData.Dependency.data();
}

void VulkanRenderPass::CreateRenderPass() {
    if (vkCreateRenderPass(VulkanDeviceManager::GetSelectedDevice(), &renderPassData.RenderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
        Debug::Error("Failed To Create Render Pass!");
}

void VulkanRenderPass::Clean() {
    vkDestroyRenderPass(VulkanDeviceManager::GetSelectedDevice(), renderPass, nullptr);
}