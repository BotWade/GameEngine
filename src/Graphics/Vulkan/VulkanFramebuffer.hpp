#ifndef _VULKANFRAMEBUFFER_
#define _VULKANFRAMEBUFFER_

#include "../../Math/Vector2.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

class VulkanRenderPass;

class VulkanFramebuffer {
public:
    vector<VkFramebuffer> Framebuffers;
    VulkanRenderPass* setRenderPass;

    void CreateFramebuffers(VulkanRenderPass* renderPass);
    void CreateFramebuffers(VulkanRenderPass* renderPass, vector<vector<VkImageView>>& viewTargets, Vector2 Size);
    void Recreate();
    void Clean();
};

#endif