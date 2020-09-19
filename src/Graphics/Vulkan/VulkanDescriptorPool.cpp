#include "VulkanDescriptorPool.hpp"
#include "Vulkan.hpp"
#include "VulkanDeviceManager.hpp"
#include "../../Core/Debug.hpp"
#include "../Shader.hpp"

void VulkanDescriptorPool::CreateDescriptorPool(Shader* shader, size_t FrameBufferSize) {
    vector<VkDescriptorPoolSize> poolSizes;

    for (Uniform* uniform : shader->Uniforms) {
        VkDescriptorPoolSize poolSize{};
        
        switch (uniform->type) {
            case UNIFORM_BUFFER:
                poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                break;
            case UNIFORM_TEXTURE:
                poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                break;
        }
        
        poolSize.descriptorCount = static_cast<uint32_t>(FrameBufferSize);
        poolSizes.push_back(poolSize);    
    }
    
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(FrameBufferSize);

    if (vkCreateDescriptorPool(VulkanDeviceManager::GetSelectedDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
        Debug::Error("failed to create descriptor pool!");
}

void VulkanDescriptorPool::Clean() {
    vkDestroyDescriptorPool(VulkanDeviceManager::GetSelectedDevice(), descriptorPool, nullptr);
}