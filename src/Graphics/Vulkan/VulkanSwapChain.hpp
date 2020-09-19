#ifndef _VULKANSWAPCHAIN_
#define _VULKANSWAPCHAIN_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentModes;

    inline bool IsSwapChainAdequate();
};

class VulkanSwapChain {
public:
    static SwapChainSupportDetails QuerySwapChainSupport(size_t Index);
    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR ChooseSwapPresentMode(vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D ChooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities);
};

inline bool SwapChainSupportDetails::IsSwapChainAdequate() {
    return !formats.empty() && !presentModes.empty();
}

#endif