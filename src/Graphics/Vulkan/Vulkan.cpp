#include "Vulkan.hpp"
#include "VulkanValidationLayer.hpp"
#include "VulkanDeviceManager.hpp"
#include "VulkanSemaphore.hpp"
#include "PipelineManager.hpp"
#include "RenderPassManager.hpp"
#include "../../Core/Window.hpp"
#include "../../Core/Debug.hpp"
#include "../Renderer.hpp"
#include "VulkanDescriptorPool.hpp"

#define DEBUG
#ifdef NDEBUG
    bool Vulkan::enableValidationLayers = false;
#else
    bool Vulkan::enableValidationLayers = true;
#endif

VkInstance Vulkan::instance;
VkSurfaceKHR Vulkan::surface;
VkDebugUtilsMessengerEXT Vulkan::debugMessenger;
SwapChainData Vulkan::swapChainData;
int Vulkan::MAX_FRAMES_IN_FLIGHT = 2;
size_t Vulkan::CurrentFrame = 0;
VulkanFramebuffer* Vulkan::frameBuffer;
VulkanCommandBuffer* Vulkan::commandBuffer;
size_t Vulkan::CurrentCommandBuffer;
PFN_vkCmdSetDepthCompareOpEXT Vulkan::vkCmdSetDepthCompareOp;
PFN_vkCmdSetDepthTestEnableEXT Vulkan::vkCmdSetDepthTestEnable;
VulkanDescriptorPool* Vulkan::descriptorPool;
uint32_t Vulkan::imageIndex = 0;
bool Vulkan::SwapChainRecreated;
VkImage Vulkan::depthImage;
VkDeviceMemory Vulkan::depthImageMemory;
VkImageView Vulkan::depthImageView;

void Vulkan::CreateInstance(InstanceData appData) {
    if (enableValidationLayers && !VulkanValidationLayer::CheckValidationLayerSupport())
        Debug::Error("Validation Layers Requested, But Not Available!");

    VkApplicationInfo vkInfo{};
    vkInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vkInfo.pApplicationName = appData.ApplicationName.c_str();
    vkInfo.pEngineName = appData.EngineName.c_str();
    vkInfo.applicationVersion = appData.ApplicationVersion;
    vkInfo.engineVersion = appData.EngineVersion;
    vkInfo.apiVersion = VK_API_VERSION_1_2;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &vkInfo;

    vector<const char *> extensions = VulkanValidationLayer::GetRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    
    if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(VulkanValidationLayer::validationLayers.size());
            createInfo.ppEnabledLayerNames = VulkanValidationLayer::validationLayers.data();

            VulkanValidationLayer::PopulateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
        Debug::Error("Failed To Create Instance!");

    vkCmdSetDepthCompareOp = reinterpret_cast<PFN_vkCmdSetDepthCompareOpEXT>(vkGetInstanceProcAddr(instance, "vkCmdSetDepthCompareOpEXT"));
    vkCmdSetDepthTestEnable = reinterpret_cast<PFN_vkCmdSetDepthTestEnableEXT>(vkGetInstanceProcAddr(instance, "vkCmdSetDepthTestEnableEXT"));
}

void Vulkan::CreateSurface() {
    if (glfwCreateWindowSurface(instance, Window::window, nullptr, &surface))
        Debug::Error("Failed To Create Window Surface");
}

void Vulkan::CreateDepthResources() {
    VkFormat depthFormat = FindDepthFormat();
    CreateImage(Vector2(swapChainData.Extent.width, swapChainData.Extent.height), depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    depthImageView = CreateImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
    //TransitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
}

VkFormat Vulkan::FindSupportedFormat(const vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(VulkanDeviceManager::GetSelectedVkPhysicalDevice(), format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
            return format;
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
            return format;
    }

    Debug::Error("Failed To Find Supported Format!");
    return VK_FORMAT_UNDEFINED;
}

VkFormat Vulkan::FindDepthFormat() {
    return FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool Vulkan::HasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void Vulkan::SetupDebugMessenger() {

    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    VulkanValidationLayer::PopulateDebugMessengerCreateInfo(createInfo);

    if (VulkanValidationLayer::CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
        Debug::Error("failed to set up debug messenger!");
}

void Vulkan::CreateSwapChain() {
    swapChainData.SupportDetails = VulkanSwapChain::QuerySwapChainSupport(VulkanDeviceManager::selectedPhysicalDevice);
    swapChainData.SurfaceFormat = VulkanSwapChain::ChooseSwapSurfaceFormat(swapChainData.SupportDetails.formats);
    swapChainData.PresentMode = VulkanSwapChain::ChooseSwapPresentMode(swapChainData.SupportDetails.presentModes);
    swapChainData.Extent = VulkanSwapChain::ChooseSwapExtent(swapChainData.SupportDetails.capabilities);
    swapChainData.ImageCount = swapChainData.SupportDetails.capabilities.minImageCount + 1;
    
    if (swapChainData.SupportDetails.capabilities.maxImageCount > 0 && swapChainData.ImageCount > swapChainData.SupportDetails.capabilities.maxImageCount)
        swapChainData.ImageCount = swapChainData.SupportDetails.capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    
    createInfo.minImageCount = swapChainData.ImageCount;
    createInfo.imageFormat = swapChainData.SurfaceFormat.format;
    createInfo.imageColorSpace = swapChainData.SurfaceFormat.colorSpace;
    createInfo.imageExtent = swapChainData.Extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    
    QueueFamilyIndices indices = VulkanDeviceManager::GetSelectedPhysicalDevice()->Indices;
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};
    
    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } 
    else
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

    createInfo.preTransform = swapChainData.SupportDetails.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = swapChainData.PresentMode;
    createInfo.clipped = VK_TRUE;
    
    if (vkCreateSwapchainKHR(VulkanDeviceManager::GetSelectedDevice(), &createInfo, nullptr, &swapChainData.SwapChain) != VK_SUCCESS)
        Debug::Error("failed to create swap chain!");

    vkGetSwapchainImagesKHR(VulkanDeviceManager::GetSelectedDevice(), swapChainData.SwapChain, &swapChainData.ImageCount, nullptr);
    swapChainData.SwapChainImages.resize(swapChainData.ImageCount);
    vkGetSwapchainImagesKHR(VulkanDeviceManager::GetSelectedDevice(), swapChainData.SwapChain, &swapChainData.ImageCount, swapChainData.SwapChainImages.data());

    swapChainData.SwapChainFortmat = swapChainData.SurfaceFormat.format;
}

void Vulkan::CreateImageViews() {
    size_t Size = swapChainData.SwapChainImages.size();
    
    swapChainData.SwapChainImageViews.resize(Size);
    for (size_t Index = 0; Index < Size; Index++)
        swapChainData.SwapChainImageViews[Index] = CreateImageView(swapChainData.SwapChainImages[Index], swapChainData.SwapChainFortmat, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Vulkan::PrepareFrame() {

    vkWaitForFences(VulkanDeviceManager::GetSelectedDevice(), 1, &VulkanSemaphore::inFlightFences[CurrentFrame], VK_TRUE, UINT64_MAX);

    VkResult Result = vkAcquireNextImageKHR(VulkanDeviceManager::GetSelectedDevice(), swapChainData.SwapChain, UINT64_MAX, VulkanSemaphore::imageAvailableSemaphores[CurrentFrame], VK_NULL_HANDLE, &imageIndex);
    
    if (Result == VK_ERROR_OUT_OF_DATE_KHR)  {
        Window::frameBufferResized = false;
        RecreateSwapChain();
        return;
    }
    else if (Result != VK_SUCCESS && Result != VK_SUBOPTIMAL_KHR)
        Debug::Error("Failed to Acquire Swap Chain Image!");
}

void Vulkan::DrawFrame() {

    if (SwapChainRecreated) {
        SwapChainRecreated = false;
        return;
    }

    if (VulkanSemaphore::imagesInFlight[imageIndex] != VK_NULL_HANDLE)
        vkWaitForFences(VulkanDeviceManager::GetSelectedDevice(), 1, &VulkanSemaphore::imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    
    VulkanSemaphore::imagesInFlight[imageIndex] = VulkanSemaphore::inFlightFences[CurrentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {VulkanSemaphore::imageAvailableSemaphores[CurrentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = CommandBufferManager::commandPool->drawQueue.size();
    submitInfo.pCommandBuffers = CommandBufferManager::commandPool->drawQueue.data();

    VkSemaphore signalSemaphores[] = {VulkanSemaphore::renderFinishedSemaphores[CurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(VulkanDeviceManager::GetSelectedDevice(), 1, &VulkanSemaphore::inFlightFences[CurrentFrame]);

    if (vkQueueSubmit(VulkanDeviceManager::GetSelectedPhysicalDevice()->graphicsQueue, 1, &submitInfo, VulkanSemaphore::inFlightFences[CurrentFrame]) != VK_SUCCESS)
        Debug::Error("failed to submit draw command buffer!");

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChainData.SwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    
    presentInfo.pImageIndices = &imageIndex;

    VkResult Result = vkQueuePresentKHR(VulkanDeviceManager::GetSelectedPhysicalDevice()->presentQueue, &presentInfo);

    if (Result == VK_ERROR_OUT_OF_DATE_KHR || Result == VK_SUBOPTIMAL_KHR || Window::frameBufferResized) {
        Window::frameBufferResized = false;
        SwapChainRecreated = true;
        RecreateSwapChain();
    }
    else if (Result != VK_SUCCESS)
        Debug::Error("Failed To Present Swap Chain Image!");
    
    CurrentFrame = (CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Vulkan::RecreateSwapChain() {
    Vector2 Size = Window::GetSize();
    SwapChainRecreated = true;
    while (Size.X == 0 || Size.Y == 0) {
        Size = Window::GetSize();
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(VulkanDeviceManager::GetSelectedDevice());

    for (auto imageView : swapChainData.SwapChainImageViews)
        vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), imageView, nullptr);

    vkDestroySwapchainKHR(VulkanDeviceManager::GetSelectedDevice(), swapChainData.SwapChain, nullptr);

    vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), depthImageView, nullptr);
    vkDestroyImage(VulkanDeviceManager::GetSelectedDevice(), depthImage, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), depthImageMemory, nullptr);

    CreateSwapChain();
    CreateImageViews();
    CreateDepthResources();
    frameBuffer->Recreate();
}

void Vulkan::CleanSwapChain() {
    frameBuffer->Clean();
    
    PipelineManager::CleanPipelines();
    RenderPassManager::ClearRenderPass();

    for (auto imageView : swapChainData.SwapChainImageViews)
        vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), imageView, nullptr);

    vkDestroySwapchainKHR(VulkanDeviceManager::GetSelectedDevice(), swapChainData.SwapChain, nullptr);
}

void Vulkan::Clean() {
    vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), depthImageView, nullptr);
    vkDestroyImage(VulkanDeviceManager::GetSelectedDevice(), depthImage, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), depthImageMemory, nullptr);

    for (auto imageView : swapChainData.SwapChainImageViews)
        vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), imageView, nullptr);

    vkDestroySwapchainKHR(VulkanDeviceManager::GetSelectedDevice(), swapChainData.SwapChain, nullptr);
    VulkanDeviceManager::Clean();

    if (enableValidationLayers)
        VulkanValidationLayer::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);

    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
}

VkFramebuffer Vulkan::GetCurrentFramebuffer() {
    return (frameBuffer == nullptr ? VkFramebuffer() : frameBuffer->Framebuffers[CurrentCommandBuffer]);
}

VkCommandBuffer Vulkan::GetCurrentCommandBuffer() {
    return commandBuffer->commandBuffers[CurrentCommandBuffer];
}

VkImageView Vulkan::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType type) {
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = type;
    viewInfo.format = format;
    viewInfo.components = {
		VK_COMPONENT_SWIZZLE_R,
		VK_COMPONENT_SWIZZLE_G,
		VK_COMPONENT_SWIZZLE_B,
		VK_COMPONENT_SWIZZLE_A
	};
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;

    switch (type) {
        case VK_IMAGE_VIEW_TYPE_CUBE:
            viewInfo.subresourceRange.layerCount = 6;
            break;
        default:
            viewInfo.subresourceRange.layerCount = 1;
            break;
    }
    
    VkImageView imageView;
    if (vkCreateImageView(VulkanDeviceManager::GetSelectedDevice(), &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        Debug::Error("failed to create texture image view!");

    return imageView;
}

void Vulkan::CreateImage(Vector2 Size, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& Image, VkDeviceMemory& ImageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = Size.X;
    imageInfo.extent.height = Size.Y;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    
    if (vkCreateImage(VulkanDeviceManager::GetSelectedDevice(), &imageInfo, nullptr, &Image) != VK_SUCCESS)
        Debug::Error("Failed To Create Texture!");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(VulkanDeviceManager::GetSelectedDevice(), Image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Renderer::FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(VulkanDeviceManager::GetSelectedDevice(), &allocInfo, nullptr, &ImageMemory) != VK_SUCCESS)
        Debug::Error("failed to allocate image memory!");

    vkBindImageMemory(VulkanDeviceManager::GetSelectedDevice(), Image, ImageMemory, 0);
}

void Vulkan::CreateCubeMapImage(Vector2 Size, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& Image, VkDeviceMemory& ImageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = Size.X;
    imageInfo.extent.height = Size.Y;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.arrayLayers = 6;
	imageInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

    if (vkCreateImage(VulkanDeviceManager::GetSelectedDevice(), &imageInfo, nullptr, &Image) != VK_SUCCESS)
        Debug::Error("Failed To Create Texture!");

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(VulkanDeviceManager::GetSelectedDevice(), Image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Renderer::FindMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(VulkanDeviceManager::GetSelectedDevice(), &allocInfo, nullptr, &ImageMemory) != VK_SUCCESS)
        Debug::Error("failed to allocate image memory!");

    vkBindImageMemory(VulkanDeviceManager::GetSelectedDevice(), Image, ImageMemory, 0);
}

void Vulkan::TransitionImageLayout(VkImage& image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange* subresourceRange, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask) {
    SingleTimeCommandData Data = Renderer::BeginSingleTimeCommands();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    if (subresourceRange == nullptr) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
    }
    else
        barrier.subresourceRange = *subresourceRange;
    
    switch (oldLayout) {
		case VK_IMAGE_LAYOUT_UNDEFINED:
			barrier.srcAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
        case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
            barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            break;
		default:
			Debug::Error("unsupported layout transition!");
			break;
	}

	switch (newLayout) {
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			barrier.dstAccessMask = barrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (barrier.srcAccessMask == 0)
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			Debug::Error("unsupported layout transition!");
			break;
	}

    vkCmdPipelineBarrier(
        Data.commandBuffer,
        srcStageMask, dstStageMask,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    Renderer::EndSingleTimeCommands(Data);
}