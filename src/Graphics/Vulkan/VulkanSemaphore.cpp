#include "VulkanSemaphore.hpp"
#include "VulkanDeviceManager.hpp"
#include "Vulkan.hpp"
#include "../../Core/Debug.hpp"

vector<VkSemaphore> VulkanSemaphore::imageAvailableSemaphores;
vector<VkSemaphore> VulkanSemaphore::renderFinishedSemaphores;
vector<VkFence> VulkanSemaphore::inFlightFences;
vector<VkFence> VulkanSemaphore::imagesInFlight;

void VulkanSemaphore::CreateSemaphores() {
    imageAvailableSemaphores.resize(Vulkan::MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(Vulkan::MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(Vulkan::MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(Vulkan::swapChainData.SwapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    
    for (size_t Index = 0; Index < Vulkan::MAX_FRAMES_IN_FLIGHT; Index++)
        if (vkCreateSemaphore(VulkanDeviceManager::GetSelectedDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[Index]) != VK_SUCCESS ||
            vkCreateSemaphore(VulkanDeviceManager::GetSelectedDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[Index]) != VK_SUCCESS ||
            vkCreateFence(VulkanDeviceManager::GetSelectedDevice(), &fenceInfo, nullptr, &inFlightFences[Index]) != VK_SUCCESS)
                Debug::Error("failed to create synchronization objects for a frame!");
}

void VulkanSemaphore::Clean() {
    for (size_t Index = 0; Index < Vulkan::MAX_FRAMES_IN_FLIGHT; Index++) {
        vkDestroySemaphore(VulkanDeviceManager::GetSelectedDevice(), renderFinishedSemaphores[Index], nullptr);
        vkDestroySemaphore(VulkanDeviceManager::GetSelectedDevice(), imageAvailableSemaphores[Index], nullptr);
        vkDestroyFence(VulkanDeviceManager::GetSelectedDevice(), inFlightFences[Index], nullptr);
    }
}