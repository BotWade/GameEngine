#include "VulkanPipeLine.hpp"
#include "Vulkan.hpp"
#include "VulkanDeviceManager.hpp"
#include "../../Core/Debug.hpp"
#include "../Shader.hpp"
#include "VulkanDescriptor.hpp"

void VulkanPipeLine::PreparePipelineLayout() {

    vkPipeLineData.VertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vkPipeLineData.VertexInputInfo.vertexBindingDescriptionCount = 0;
    vkPipeLineData.VertexInputInfo.vertexAttributeDescriptionCount = 0;

    vkPipeLineData.InputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    vkPipeLineData.InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    vkPipeLineData.InputAssembly.primitiveRestartEnable = VK_FALSE;

    vkPipeLineData.Viewport.x = 0.0f;
    vkPipeLineData.Viewport.y = 0.0f;
    vkPipeLineData.Viewport.minDepth = 0.0f;
    vkPipeLineData.Viewport.maxDepth = 1.0f;

    vkPipeLineData.Scissor.offset = {0, 0};
    vkPipeLineData.Scissor.extent = {0, 0};

    vkPipeLineData.ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vkPipeLineData.ViewportState.viewportCount = 1;
    vkPipeLineData.ViewportState.scissorCount = 1;

    vkPipeLineData.Rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    vkPipeLineData.Rasterizer.depthClampEnable = VK_FALSE;
    vkPipeLineData.Rasterizer.rasterizerDiscardEnable = VK_FALSE;
    vkPipeLineData.Rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    vkPipeLineData.Rasterizer.lineWidth = 1.0f;
    vkPipeLineData.Rasterizer.cullMode = VK_CULL_MODE_NONE;
    vkPipeLineData.Rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    vkPipeLineData.Rasterizer.depthBiasEnable = VK_FALSE;

    vkPipeLineData.Multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    vkPipeLineData.Multisampling.sampleShadingEnable = VK_FALSE;
    vkPipeLineData.Multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    vkPipeLineData.DepthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    vkPipeLineData.DepthStencil.depthTestEnable = VK_TRUE;
    vkPipeLineData.DepthStencil.depthWriteEnable = VK_TRUE;
    vkPipeLineData.DepthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    vkPipeLineData.DepthStencil.depthBoundsTestEnable = VK_FALSE;
    vkPipeLineData.DepthStencil.stencilTestEnable = VK_FALSE;

	vkPipeLineData.ColorBlendAttachment.blendEnable = VK_TRUE;
	vkPipeLineData.ColorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	vkPipeLineData.ColorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	vkPipeLineData.ColorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	vkPipeLineData.ColorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	vkPipeLineData.ColorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	vkPipeLineData.ColorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	vkPipeLineData.ColorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    vkPipeLineData.ColorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    vkPipeLineData.ColorBlend.attachmentCount = 1;
    vkPipeLineData.ColorBlend.pAttachments = &vkPipeLineData.ColorBlendAttachment;
    vkPipeLineData.ColorBlend.blendConstants[0] = 0.0f;
    vkPipeLineData.ColorBlend.blendConstants[1] = 0.0f;
    vkPipeLineData.ColorBlend.blendConstants[2] = 0.0f;
    vkPipeLineData.ColorBlend.blendConstants[3] = 0.0f;

    vkPipeLineData.PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    vkPipeLineData.PipelineLayoutInfo.setLayoutCount = 0;
    vkPipeLineData.PipelineLayoutInfo.pushConstantRangeCount = 0;

    vkPipeLineData.dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
    vkPipeLineData.dynamicState.dynamicStateCount = static_cast<uint32_t>(vkPipeLineData.dynamicStateEnables.size());
    vkPipeLineData.dynamicState.pDynamicStates = vkPipeLineData.dynamicStateEnables.data();
    vkPipeLineData.dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    vkPipeLineData.dynamicState.flags = 0;
}

void VulkanPipeLine::SetVertexInputInfo(vector<VkVertexInputBindingDescription>* bindingDesc, vector<VkVertexInputAttributeDescription>* attributeDesc) {
    vkPipeLineData.VertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDesc->size());
    vkPipeLineData.VertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDesc->size());
    vkPipeLineData.VertexInputInfo.pVertexBindingDescriptions = bindingDesc->data();
    vkPipeLineData.VertexInputInfo.pVertexAttributeDescriptions = attributeDesc->data();
}

void VulkanPipeLine::CreatePipelineLayout(VulkanDescriptor* vulkanDescriptor) {
    
    ///When Resizing The Swapchain we need to recreate the pipeline layout, because only the viewport only the width and height change we seet this values here
    ///We check if width is 0 so that if the dev as set a custom size it dosent override

    if (vkPipeLineData.Scissor.extent.width == 0)
        vkPipeLineData.Scissor.extent = { vkPipeLineData.Viewport.width, vkPipeLineData.Viewport.height};

    if (vulkanDescriptor != nullptr) {
        vkPipeLineData.PipelineLayoutInfo.setLayoutCount = 1;
        vkPipeLineData.PipelineLayoutInfo.pSetLayouts = &vulkanDescriptor->descriptorSetLayout;
    }

    if (vkCreatePipelineLayout(VulkanDeviceManager::GetSelectedDevice(), &vkPipeLineData.PipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        Debug::Error("failed to create pipeline layout!");
}

void VulkanPipeLine::CreatePipeline(Shader* shader, VulkanRenderPass* renderPass) {
    
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shader->shaderStages.size());
    pipelineInfo.pStages = shader->shaderStages.data();
    pipelineInfo.pVertexInputState = &vkPipeLineData.VertexInputInfo;
    pipelineInfo.pInputAssemblyState = &vkPipeLineData.InputAssembly;
    pipelineInfo.pViewportState = &vkPipeLineData.ViewportState;
    pipelineInfo.pRasterizationState = &vkPipeLineData.Rasterizer;
    pipelineInfo.pMultisampleState = &vkPipeLineData.Multisampling;
    pipelineInfo.pDepthStencilState = &vkPipeLineData.DepthStencil;
    pipelineInfo.pColorBlendState = &vkPipeLineData.ColorBlend;
    pipelineInfo.pDynamicState = &vkPipeLineData.dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass->renderPass;
    pipelineInfo.basePipelineIndex = -1;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    currentRenderPass = renderPass;
    currentShader = shader;

    if (vkCreateGraphicsPipelines(VulkanDeviceManager::GetSelectedDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
        Debug::Error("failed to create graphics pipeline!");
}

void VulkanPipeLine::Bind() {
    vkCmdBindPipeline(Vulkan::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}

void VulkanPipeLine::Clean() {
    vkDestroyPipeline(VulkanDeviceManager::GetSelectedDevice(), graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(VulkanDeviceManager::GetSelectedDevice(), pipelineLayout, nullptr);
}