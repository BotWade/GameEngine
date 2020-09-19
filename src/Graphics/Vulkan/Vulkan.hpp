#ifndef _VULKAN_
#define _VULKAN_

#include "VulkanSwapChain.hpp"
#include "VulkanCommandBuffer.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanFramebuffer.hpp"
#include "VulkanQueueFamily.hpp"
#include "CommandBufferManager.hpp"
#include "../../Math/Vector2.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

using namespace std;

struct InstanceData {
    string ApplicationName = "Vulkan";
    string EngineName = "GameEngine";
    uint32_t ApplicationVersion;
    uint32_t EngineVersion;
};

struct SwapChainData {
    SwapChainSupportDetails SupportDetails;
    VkSurfaceFormatKHR SurfaceFormat;
    VkPresentModeKHR PresentMode;
    VkFormat SwapChainFortmat;
    VkExtent2D Extent;
    uint32_t ImageCount;
    VkSwapchainKHR SwapChain;
    vector<VkImage> SwapChainImages;
    vector<VkImageView> SwapChainImageViews;
};

class VulkanDescriptorPool;

class Vulkan {
public:
    ///External Functions
    static PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOp;
    static PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnable;
    
    static bool SwapChainRecreated;
    static int MAX_FRAMES_IN_FLIGHT;
    static size_t CurrentFrame;
    static bool enableValidationLayers;
    static VkInstance instance;
    static VkSurfaceKHR surface;
    static VkDebugUtilsMessengerEXT debugMessenger;
    static SwapChainData swapChainData;
    static VulkanRenderPass* renderPass;
    static VulkanFramebuffer* frameBuffer;
    static VulkanCommandBuffer* commandBuffer;
    static VulkanDescriptorPool* descriptorPool;
    static size_t CurrentCommandBuffer;
    static uint32_t imageIndex;
    static VkImage depthImage;
    static VkDeviceMemory depthImageMemory;
    static VkImageView depthImageView;

    static void CreateInstance(InstanceData appData);
    static void CreateSurface();
    static void SetupDebugMessenger();
    static void CreateSwapChain();
    static void CreateImageViews();
    static void CreateDepthResources();
    static VkFormat FindSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    static VkFormat FindDepthFormat();
    static bool HasStencilComponent(VkFormat format);
    static void DrawFrame();
    static void PrepareFrame();
    static void CleanSwapChain();
    static void RecreateSwapChain();
    static void Clean();
    static VkCommandBuffer GetCurrentCommandBuffer();
    static VkFramebuffer GetCurrentFramebuffer();
    static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType = VK_IMAGE_VIEW_TYPE_2D);
    static void CreateImage(Vector2 Size, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& textureImage, VkDeviceMemory& imageMemory);
    static void CreateCubeMapImage(Vector2 Size, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& Image, VkDeviceMemory& ImageMemory);
    static void TransitionImageLayout(VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange* subresourceRange = nullptr, VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);
};

#endif