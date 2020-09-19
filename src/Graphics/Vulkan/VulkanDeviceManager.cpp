#include "VulkanDeviceManager.hpp"
#include "Vulkan.hpp"
#include "VulkanValidationLayer.hpp"

#include "../../Core/Debug.hpp"

vector<VulkanPhysicalDevice*> VulkanDeviceManager::physicalDevices;
size_t VulkanDeviceManager::selectedPhysicalDevice = SIZE_MAX;
vector<const char*> VulkanDeviceManager::deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

void VulkanDeviceManager::UpdatePhysicalDevices() {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(Vulkan::instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        Debug::Error("Failed To Find GPUs With Vulkan Support!");

    vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(Vulkan::instance, &deviceCount, devices.data());
  

    for (size_t Index = 0; Index < deviceCount; Index++) {
        
        VulkanPhysicalDevice* physicalDevice = new VulkanPhysicalDevice();
        physicalDevice->physicalDevice = devices[Index];
        vkGetPhysicalDeviceProperties(physicalDevice->physicalDevice, &physicalDevice->properties);
        vkGetPhysicalDeviceFeatures(physicalDevice->physicalDevice, &physicalDevice->features);
        physicalDevice->features.samplerAnisotropy = VK_TRUE;
        
        physicalDevices.push_back(physicalDevice);

        Debug::Log("Found Vulkan Device - " + (string)physicalDevice->properties.deviceName);

        if (IsDeviceSuitable(Index))
            selectedPhysicalDevice = Index;
    }

    if (selectedPhysicalDevice == SIZE_MAX)
        Debug::Error("Failed To Find A Suitable GPU!");
}

void VulkanDeviceManager::CreateLogicalDevice() {

    VulkanPhysicalDevice* physicalDevice = GetSelectedPhysicalDevice();

    vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    set<uint32_t> uniqueQueueFamilies = { physicalDevice->Indices.graphicsFamily.value(), physicalDevice->Indices.presentFamily.value() };

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = physicalDevice->Indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    
    createInfo.pEnabledFeatures = &physicalDevice->features;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (Vulkan::enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValidationLayer::validationLayers.size());
        createInfo.ppEnabledLayerNames = VulkanValidationLayer::validationLayers.data();
    }
    else
        createInfo.enabledLayerCount = 0;

    if (vkCreateDevice(physicalDevice->physicalDevice, &createInfo, nullptr, &physicalDevice->device) != VK_SUCCESS)
        Debug::Error("Failed To Create Logical Device");

    vkGetDeviceQueue(physicalDevice->device, physicalDevice->Indices.graphicsFamily.value(), 0, &physicalDevice->graphicsQueue);
    vkGetDeviceQueue(physicalDevice->device, physicalDevice->Indices.presentFamily.value(), 0, &physicalDevice->presentQueue);
}

QueueFamilyIndices* VulkanPhysicalDevice::UpdateQueueFamilies() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    int Index = 0;

    for (const VkQueueFamilyProperties queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            Indices.graphicsFamily = Index;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, Index, Vulkan::surface, &presentSupport);

        if (presentSupport)
            Indices.presentFamily = Index;

        if (Indices.IsComplete())
            break;

        Index++;
    }

    return &Indices;
}

bool VulkanDeviceManager::CheckDeviceExtensionSupport(size_t Index) {
    VulkanPhysicalDevice* device = physicalDevices[Index];
    
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device->physicalDevice, nullptr, &extensionCount, nullptr);
    vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device->physicalDevice, nullptr, &extensionCount, availableExtensions.data());

    set<string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions)
        requiredExtensions.erase(extension.extensionName);

    return requiredExtensions.empty();
}

void VulkanDeviceManager::Clean() {
    for (VulkanPhysicalDevice* physicalDevice : physicalDevices) {
        vkDestroyDevice(physicalDevice->device, nullptr);
    }
}