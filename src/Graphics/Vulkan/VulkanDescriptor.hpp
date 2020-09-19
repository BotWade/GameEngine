#ifndef _VULKANDESCRIPTOR_
#define _VULKANDESCRIPTOR_

#include "Vulkan.hpp"
#include "VulkanPipeLine.hpp"
#include "../Uniform.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

class Texture;

struct UniformData {
    string Name;
    VkDescriptorSetLayoutBinding LayoutBinding;
    vector<VkBuffer> uniformBuffers;
    vector<VkDeviceMemory> uniformBuffersMemory;
    size_t uniformSize;
    UniformType type;
    Texture* texture;

    UniformData(Uniform uniform);
    void Generate(size_t UniformSize, size_t FrameBufferSize = Vulkan::swapChainData.SwapChainImages.size());
    void UpdateUniform(void* Data, size_t Index = Vulkan::imageIndex);
    void SetTexture(Texture* newTexture);
    void Clean();
};

inline void UniformData::SetTexture(Texture* newTexture) {
    texture = newTexture;
}

class VulkanDescriptor {
public:
    vector<UniformData*> uniformData;
    vector<VkDescriptorSet> descriptorSets;
    VulkanDescriptorPool* descriptorPool;
    VkDescriptorSetLayout descriptorSetLayout;

    void PrepareUniforms(Shader* shader);
    void Generate(Shader* shader, size_t FrameBufferSize = Vulkan::swapChainData.SwapChainImages.size());
    void Bind(VulkanPipeLine* pipeLine);
    UniformData* GetUniform(string Name);
    void Clean();
};

inline void VulkanDescriptor::Bind(VulkanPipeLine* pipeLine) {
    vkCmdBindDescriptorSets(Vulkan::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine->pipelineLayout, 0, 1, &descriptorSets[Vulkan::CurrentCommandBuffer], 0, nullptr);
}

#endif