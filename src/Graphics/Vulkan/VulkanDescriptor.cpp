#include "VulkanDescriptor.hpp"
#include "../../Core/Debug.hpp"
#include "VulkanDeviceManager.hpp"
#include "../Renderer.hpp"
#include "../Shader.hpp"

UniformData::UniformData(Uniform uniform) {
    type = uniform.type;
    LayoutBinding = uniform.LayoutBinding;
    Name = uniform.Name;
}

void UniformData::Generate(size_t UniformSize, size_t FrameBufferSize) {

    uniformSize = UniformSize;
    size_t Size = FrameBufferSize;
    uniformBuffers.resize(Size);
    uniformBuffersMemory.resize(Size);

    for (size_t Index = 0; Index < Size; Index++)
        Renderer::CreateBuffer(UniformSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[Index], uniformBuffersMemory[Index]);
}

void UniformData::UpdateUniform(void* Data, size_t Index) {

    void* vkData;
    vkMapMemory(VulkanDeviceManager::GetSelectedDevice(), uniformBuffersMemory[Index], 0, uniformSize, 0, &vkData);
    memcpy(vkData, Data, uniformSize);
    vkUnmapMemory(VulkanDeviceManager::GetSelectedDevice(), uniformBuffersMemory[Index]);
}

void UniformData::Clean() {
    size_t Size = uniformBuffers.size();
    for (size_t Index = 0; Index < Size; Index++) {
        vkDestroyBuffer(VulkanDeviceManager::GetSelectedDevice(), uniformBuffers[Index], nullptr);
        vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), uniformBuffersMemory[Index], nullptr);
    }
}

void VulkanDescriptor::PrepareUniforms(Shader* shader) {
    for (Uniform* uniform : shader->Uniforms)
        uniformData.push_back(new UniformData(*uniform));
}

void VulkanDescriptor::Generate(Shader* shader, size_t FrameBufferSize) {
    vector<VkDescriptorSetLayoutBinding> LayoutBindings = vector<VkDescriptorSetLayoutBinding>(uniformData.size());

    for (size_t Index = 0; Index < LayoutBindings.size(); Index++)
        LayoutBindings[Index] = uniformData[Index]->LayoutBinding;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(LayoutBindings.size());
    layoutInfo.pBindings = LayoutBindings.data();

    if (vkCreateDescriptorSetLayout(VulkanDeviceManager::GetSelectedDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
        Debug::Error("failed to create descriptor set layout!");

    size_t Size = FrameBufferSize;
    std::vector<VkDescriptorSetLayout> layouts(Size, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorPool = new VulkanDescriptorPool();
    descriptorPool->CreateDescriptorPool(shader, FrameBufferSize);
    allocInfo.descriptorPool = descriptorPool->descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(Size);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(Size);

    VkResult Result = vkAllocateDescriptorSets(VulkanDeviceManager::GetSelectedDevice(), &allocInfo, descriptorSets.data());
    if (Result != VK_SUCCESS)
        Debug::Error("failed to allocate descriptor sets!");

    for (size_t Index = 0; Index < Size; Index++) {
        vector<VkWriteDescriptorSet> descriptorWrites;

        for (size_t SubIndex = 0; SubIndex < uniformData.size(); SubIndex++) {
            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[Index];
            descriptorWrite.dstBinding = SubIndex;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorCount = 1;

            switch (uniformData[SubIndex]->type) {
                case UNIFORM_BUFFER: {
                    VkDescriptorBufferInfo* bufferInfo = new VkDescriptorBufferInfo();
                    bufferInfo->buffer = uniformData[SubIndex]->uniformBuffers[Index];
                    bufferInfo->offset = 0;
                    bufferInfo->range = uniformData[SubIndex]->uniformSize;
                    
                    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    descriptorWrite.pBufferInfo = bufferInfo;
                    }
                    break;
                case UNIFORM_TEXTURE: {
                    VkDescriptorImageInfo* imageInfo = new VkDescriptorImageInfo();
                    imageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

                    switch (uniformData[SubIndex]->texture->textureType) {
                        case TEXTURE2D:
                            imageInfo->imageView = uniformData[SubIndex]->texture->textureImageView;
                            break;
                        case HDR:
                            if (uniformData[SubIndex]->texture->hdrImageView == nullptr)
                                imageInfo->imageView = uniformData[SubIndex]->texture->textureImageView;
                            else
                                imageInfo->imageView = uniformData[SubIndex]->texture->hdrImageView;
                            break;
                    }
                    imageInfo->sampler = uniformData[SubIndex]->texture->textureSampler;

                    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descriptorWrite.pImageInfo = imageInfo;
                    }
                    break;
            }

            descriptorWrites.push_back(descriptorWrite);
        }
        vkUpdateDescriptorSets(VulkanDeviceManager::GetSelectedDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);    
    }
}

UniformData* VulkanDescriptor::GetUniform(string Name) {
    for (UniformData* data : uniformData)
        if (data->Name == Name)
            return data;

    return nullptr;
}

void VulkanDescriptor::Clean() {
    for (UniformData* uniform : uniformData)
        uniform->Clean();

    vkDestroyDescriptorSetLayout(VulkanDeviceManager::GetSelectedDevice(), descriptorSetLayout, nullptr);
    descriptorPool->Clean();
}