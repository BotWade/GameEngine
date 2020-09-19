#ifndef _VULKANPIPELINE_
#define _VULKANPIPELINE_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VulkanRenderPass.hpp"
#include <vector>
#include <string>

using namespace std;

struct VulkanPipeLineData {
    VkPipelineVertexInputStateCreateInfo VertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo InputAssembly;
    VkViewport Viewport;
    VkRect2D Scissor;
    VkPipelineViewportStateCreateInfo ViewportState;
    VkPipelineRasterizationStateCreateInfo Rasterizer;
    VkPipelineMultisampleStateCreateInfo Multisampling;
    VkPipelineColorBlendAttachmentState ColorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo ColorBlend;
    VkPipelineLayoutCreateInfo PipelineLayoutInfo;
    VkPipelineDepthStencilStateCreateInfo DepthStencil;
    VkPipelineDynamicStateCreateInfo dynamicState;
    vector<VkDynamicState> dynamicStateEnables;
};

class Shader;
class VulkanDescriptor;

class VulkanPipeLine {
public:
    VulkanPipeLineData vkPipeLineData;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VulkanRenderPass* currentRenderPass;
    Shader* currentShader;
    string Name;

    void PreparePipelineLayout();
    void SetVertexInputInfo(vector<VkVertexInputBindingDescription>*, vector<VkVertexInputAttributeDescription>* attributeDesc);
    void CreatePipelineLayout(VulkanDescriptor* vulkanDescriptor = nullptr);
    void CreatePipeline(Shader* shader, VulkanRenderPass* renderPass);
    void Bind();
    void Clean();
};

#endif