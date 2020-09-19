#ifndef _VULKANRENDERPASS_
#define _VULKANRENDERPASS_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <array>
#include <vector>

using namespace std;

struct VulkanRenderPassData {
    array<VkAttachmentDescription, 2> attachments;
    VkAttachmentDescription ColorAttachment;
    VkAttachmentReference ColorAttachmentRef;
    VkSubpassDescription Subpass;
    VkRenderPassCreateInfo RenderPassInfo;
    vector<VkSubpassDependency> Dependency;
    VkAttachmentDescription DepthAttachment;
    VkAttachmentReference DepthAttachmentRef;
};


class VulkanRenderPass {
public:
    VulkanRenderPassData renderPassData;
    VkRenderPass renderPass;
    string Name;
    
    void PrepareRenderPass();
    void CreateRenderPass();
    void Clean();
};

#endif