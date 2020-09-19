#ifndef _VULKANDEVICEMANAGER_
#define _VULKANDEVICEMANAGER_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <set>

#include "VulkanQueueFamily.hpp"
#include "VulkanSwapChain.hpp"

using namespace std;

struct VulkanPhysicalDevice {
    QueueFamilyIndices Indices;
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    QueueFamilyIndices* UpdateQueueFamilies();
};


class VulkanDeviceManager {
public:
    static vector<VulkanPhysicalDevice*> physicalDevices;
    static size_t selectedPhysicalDevice;
    static vector<const char*> deviceExtensions;

    static inline VulkanPhysicalDevice* GetSelectedPhysicalDevice();
    static inline VkPhysicalDevice GetSelectedVkPhysicalDevice();
    static inline VkDevice GetSelectedDevice();
    static inline VkQueue GetSelectedGraphicsQueue();
    static inline bool IsDeviceSuitable(size_t Index);
    static void UpdatePhysicalDevices();
    static void CreateLogicalDevice();
    static bool CheckDeviceExtensionSupport(size_t Index);
    static void Clean();
};

inline VulkanPhysicalDevice* VulkanDeviceManager::GetSelectedPhysicalDevice() {
    return physicalDevices[selectedPhysicalDevice];
}


inline VkPhysicalDevice VulkanDeviceManager::GetSelectedVkPhysicalDevice() {
    return physicalDevices[selectedPhysicalDevice]->physicalDevice;
}

inline VkDevice VulkanDeviceManager::GetSelectedDevice() {
    return physicalDevices[selectedPhysicalDevice]->device;
}

inline VkQueue VulkanDeviceManager::GetSelectedGraphicsQueue() {
    return physicalDevices[selectedPhysicalDevice]->graphicsQueue;
}

inline bool VulkanDeviceManager::IsDeviceSuitable(size_t Index) {
    
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevices[Index]->physicalDevice, &supportedFeatures);
    return physicalDevices[Index]->UpdateQueueFamilies()->IsComplete() && CheckDeviceExtensionSupport(Index) && VulkanSwapChain::QuerySwapChainSupport(Index).IsSwapChainAdequate() && supportedFeatures.samplerAnisotropy;
}

#endif