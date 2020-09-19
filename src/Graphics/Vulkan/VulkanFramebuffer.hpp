#ifndef _VULKANFRAMEBUFFER_
#define _VULKANFRAMEBUFFER_

#include "VulkanRenderPass.hpp"

#include "../../Math/Vector2.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

class VulkanFramebuffer {
public:
    vector<VkFramebuffer> Framebuffers;

    void CreateFramebuffers(VulkanRenderPass* renderPass);
    void CreateFramebuffers(VulkanRenderPass* renderPass, vector<vector<VkImageView>>& viewTargets, Vector2 Size);
    void Clean();
};

#endif