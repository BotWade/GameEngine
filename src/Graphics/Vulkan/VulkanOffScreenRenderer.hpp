#ifndef _VULKANOFFSCREENRENDERER_
#define _VULKANOFFSCREENRENDERER_

#include "VulkanFramebuffer.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

using namespace std;

class VulkanOffScreenRenderer {
public:
    string Name;
    bool Initialized;
    VulkanFramebuffer frameBuffer;
    vector<VkImage> image;
    vector<VkDeviceMemory> imageMemory;
    vector<VkImageView> imageViews;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    void Init(size_t SizeOfFramebuffer, Vector2 Size, VulkanRenderPass* renderPass);
    void Draw(vector<VkCommandBuffer> commandBuffers);
    void DrawAll(vector<vector<VkCommandBuffer>> commandBuffers);
    void Clean();
};

#endif