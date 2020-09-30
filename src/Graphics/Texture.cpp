#include "Texture.hpp"
#include "../Core/FileManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image"
#include "Renderer.hpp"
#include "Vulkan/VulkanDeviceManager.hpp"
#include "Vulkan/Vulkan.hpp"
#include "Vulkan/RenderPassManager.hpp"
#include "Vulkan/VulkanDescriptor.hpp"
#include "Vulkan/VulkanPipeLine.hpp"
#include "../Core/MeshManager.hpp"
#include "../Core/ShaderManager.hpp"
#include "Vulkan/PipelineManager.hpp"
#include "Vulkan/VulkanOffScreenManager.hpp"
#include <stdio.h>

bool Texture::HDRLoaderLoaded;
bool Texture::HDRLoaderLoading;
vector<Matrix4> Texture::captureViews;

void Texture::LoadTexture(string FilePath, TextureType type) {

    filePath = FilePath;
    textureType = type;

    if (!FileManager::FileExists((FileManager::GetGamePath() + FilePath).c_str()))
        Debug::Alert("Texture Image Doesnt Exit - " + FilePath);

    stbi_uc* Pixels;

    if (type == TEXTURE2D || type == HDR) {
        stbi_set_flip_vertically_on_load(1);
        Pixels = stbi_load((FileManager::GetGamePath() + FilePath).c_str(), &Width, &Height, &Channels, STBI_rgb_alpha);
    }
    else if (type == CUBEMAP) {
        Debug::Alert("Attempted To Load A Texture Of Type Cubemap (Not Implemented)");
        return;
    }

    if (!Pixels) {
        Debug::Alert("Failed To Load Texture - " + FilePath);
        return;
    }

    switch (type) {
        case TEXTURE2D:
            LoadTexture(Pixels, Vector2(Width, Height), 4);
            break;
        case HDR:
            LoadTexture(Pixels, Vector2(Width, Height), 4);
            LoadHDR();
            break;
        default:
            Debug::Alert("Invalid Texture Type");
            break;
    }

    stbi_image_free(Pixels);
}

void Texture::LoadTexture(unsigned char* Buffer, Vector2 Size, int channels) {
    Width = Size.X;
    Height = Size.Y;
    Channels = channels;

    format = static_cast<VkFormat>(VK_FORMAT_R8_SRGB + 7 * (channels == 4 ? channels : channels - 1));
    
    VkDeviceSize imageSize = Width * Height * channels;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    Renderer::CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
   
    void* vkData;
    vkMapMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory, 0, imageSize, 0, &vkData);
    memcpy(vkData, Buffer, static_cast<size_t>(imageSize));
    vkUnmapMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory);
    
    Vulkan::CreateImage(Size, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
    Vulkan::TransitionImageLayout(textureImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    CopyBufferToImage(stagingBuffer);
    Vulkan::TransitionImageLayout(textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(VulkanDeviceManager::GetSelectedDevice(), stagingBuffer, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), stagingBufferMemory, nullptr);

    CreateTextureImageView();
    CreateTextureSampler();
}

struct HDRProjectionData {
    Matrix4 Projection;
    Matrix4 View;
};

///TODO: Right Now The HDR Size Is The Same For ANY HDR Image
///
void Texture::LoadHDRLoaders(Vector2 Size) {
    HDRLoaderLoading = true;

    VulkanRenderPass* renderPass = new VulkanRenderPass();
    renderPass->PrepareRenderPass();
    renderPass->renderPassData.attachments[0].format = VK_FORMAT_R8G8B8A8_SRGB;
    renderPass->CreateRenderPass();
    renderPass->Name = "HDR";
    RenderPassManager::AddRenderPass(renderPass);

    VulkanOffScreenRenderer* offScreenRenderer = VulkanOffScreenManager::GetOffScreenRenderer("HDR");
    if (!offScreenRenderer->Initialized)
        offScreenRenderer->Init(6, Size, renderPass);

    Shader* shader = ShaderManager::GetShader("Data/Shaders/HDR.shader");
    Mesh* cube = MeshManager::GetMesh("Data/Meshes/Teste.obj");
    if (!cube->AttributesProcessed)
        cube->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    captureViews = {
       Matrix4::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3( 1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
       Matrix4::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3(-1.0f,  0.0f,  0.0f), Vector3(0.0f, -1.0f,  0.0f)),
       Matrix4::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3( 0.0f,  1.0f,  0.0f), Vector3(0.0f,  0.0f,  1.0f)),
       Matrix4::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3( 0.0f, -1.0f,  0.0f), Vector3(0.0f,  0.0f, -1.0f)),
       Matrix4::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3( 0.0f,  0.0f,  1.0f), Vector3(0.0f, -1.0f,  0.0f)),
       Matrix4::LookAt(Vector3(0.0f, 0.0f, 0.0f), Vector3( 0.0f,  0.0f, -1.0f), Vector3(0.0f, -1.0f,  0.0f))
    };

    HDRLoaderLoaded = true;
    HDRLoaderLoading = false;
}

void Texture::LoadHDR() { 
    ///MEDIUMPRIORITY TODO: Maybe Its Possible To Avoid Doing Everything Every Time We Want To Renderer A HDR Image
    ///Maybe Its Possible To Make A Global Pre Recorded CommandBuffer
    ///LOWPRIORITY TODO: Add A Option To Choose Between LH And RH Projection/View

    Width = Height = Width / 4;

    if (!HDRLoaderLoaded)
        LoadHDRLoaders(Vector2(Width, Height));

    while (HDRLoaderLoading) { /* HDR Loader Started Loading In Another Thread */ }

    ///For Consistency We Are Going To Set Width And Height Despite Both Values Being Equal 

    Shader* shader = ShaderManager::GetShader("Data/Shaders/HDR.shader");

    VulkanDescriptor uniformData;
    uniformData.PrepareUniforms(shader);
    uniformData.GetUniform("view")->Generate(sizeof(HDRProjectionData), 6);
    uniformData.GetUniform("texSampler")->SetTexture(this);
    uniformData.Generate(shader, 6);

    HDRProjectionData dm;
    dm.Projection = Matrix4::PerspectiveFov(Radians(90.0f), 1.0f, 0.1f, 10.0f);

    for (size_t Index = 0; Index < 6; Index++) {
        dm.View = captureViews[Index];
        uniformData.GetUniform("view")->UpdateUniform(&dm, Index);
    }

    VulkanPipeLine* pipeLine = new VulkanPipeLine();
    pipeLine->PreparePipelineLayout();
    pipeLine->vkPipeLineData.Viewport.height = pipeLine->vkPipeLineData.Viewport.width = Width;
    pipeLine->vkPipeLineData.Rasterizer.cullMode = VK_CULL_MODE_NONE;
    pipeLine->SetVertexInputInfo(Vertex::GetBindingDescription(), Vertex::GetAttributeDescriptions());
    pipeLine->CreatePipelineLayout(&uniformData);
    pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("HDR"));
    pipeLine->Name = "HDR";
    
    Mesh* cube = MeshManager::GetMesh("Data/Meshes/Teste.obj");
    VulkanOffScreenRenderer* offScreenRenderer = VulkanOffScreenManager::GetOffScreenRenderer("HDR");

    VulkanCommandBuffer* commandBuffer = CommandBufferManager::GetOrCreate("HDR", 6);
    
    Vulkan::commandBuffer = commandBuffer;
    
    for (size_t Index = 0; Index < commandBuffer->commandBuffers.size(); Index++) {
        Vulkan::CurrentCommandBuffer = Index;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
            Debug::Error("failed to begin recording command buffer!");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = RenderPassManager::GetRenderPass("HDR")->renderPass;
        renderPassInfo.framebuffer = offScreenRenderer->frameBuffer.Framebuffers[Index];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = { static_cast<uint32_t>(Width), static_cast<uint32_t>(Height) };


        array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(Vulkan::GetCurrentCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport = { 0.0f, 0.0f, Width, Height, 0.0f, 1.0f};
	    vkCmdSetViewport(Vulkan::GetCurrentCommandBuffer(), 0, 1, &viewport);

	    VkRect2D scissor = { { 0, 0 }, { static_cast<uint32_t>(Width), static_cast<uint32_t>(Height) }};
	    vkCmdSetScissor(Vulkan::GetCurrentCommandBuffer(), 0, 1, &scissor);

        vkCmdBindPipeline(Vulkan::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine->graphicsPipeline);

        cube->Bind();
        uniformData.Bind(pipeLine);
        cube->Draw();

        vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

        if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
            Debug::Error("failed to record command buffer!");
    }
    
    vector<vector<VkCommandBuffer>> CommandBuffers;

    for (size_t Index = 0; Index < 6; Index++)
        CommandBuffers.push_back({ commandBuffer->commandBuffers[Index] });

    offScreenRenderer->DrawAll(CommandBuffers);

    VkImageSubresourceRange range = {};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.levelCount = 1;
    range.layerCount = 6;

    Vulkan::CreateCubeMapImage(Vector2(Width, Height), format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, hdrImage, hdrImageMemory);    
    Vulkan::TransitionImageLayout(hdrImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &range);

    for (uint32_t Index = 0; Index < 6; Index++) {
        Vulkan::TransitionImageLayout(offScreenRenderer->image[Index], VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        CopyImageToImage(offScreenRenderer->image[Index], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, hdrImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, Index);
    }
    
    Vulkan::TransitionImageLayout(hdrImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, &range);

    CreateTextureImageView(HDR);
    CreateTextureSampler(HDR);

    uniformData.Clean();
    pipeLine->Clean();
}

void Texture::CopyBufferToImage(VkBuffer buffer, uint32_t targetLayer) {
    SingleTimeCommandData Data = Renderer::BeginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = targetLayer;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {0, 0, 0};
    region.imageExtent = { static_cast<uint32_t>(Width), static_cast<uint32_t>(Height), 1 };

    vkCmdCopyBufferToImage(Data.commandBuffer, buffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    Renderer::EndSingleTimeCommands(Data);
}

void Texture::CopyImageToImage(VkImage srcImage, VkImageLayout srcLayout, VkImage dstImage, VkImageLayout dstLayout, uint32_t dstLayer, uint32_t srcLayer) {
    SingleTimeCommandData Data = Renderer::BeginSingleTimeCommands();

    VkImageCopy region{};
    region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.srcSubresource.mipLevel = 0;
    region.srcSubresource.baseArrayLayer = srcLayer;
    region.srcSubresource.layerCount = 1;
    region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.dstSubresource.mipLevel = 0;
    region.dstSubresource.baseArrayLayer = dstLayer;
    region.dstSubresource.layerCount = 1;
    region.extent = { static_cast<uint32_t>(Height), static_cast<uint32_t>(Height), 1};

    vkCmdCopyImage(Data.commandBuffer, srcImage, srcLayout, dstImage, dstLayout, 1, &region);

    Renderer::EndSingleTimeCommands(Data);
}

void Texture::CreateTextureImageView(TextureType type) {
    switch (type) {
        case TEXTURE2D:
            textureImageView = Vulkan::CreateImageView(textureImage, format, VK_IMAGE_ASPECT_COLOR_BIT);   
            break;
        case HDR:
            hdrImageView = Vulkan::CreateImageView(hdrImage, format, VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_VIEW_TYPE_CUBE);
            break;
        default:
            Debug::Alert("Attempted To Create Image View For Invalid Texture Type");
            break;
    }
}

void Texture::CreateTextureSampler(TextureType type) {
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = 16.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;

    switch (type) {
        case TEXTURE2D:
            if (vkCreateSampler(VulkanDeviceManager::GetSelectedDevice(), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS)
                Debug::Error("failed to create texture sampler!");
            break;
        case HDR:
            if (vkCreateSampler(VulkanDeviceManager::GetSelectedDevice(), &samplerInfo, nullptr, &hdrSampler) != VK_SUCCESS)
                Debug::Error("failed to create texture sampler!");
            break;
        default:
            Debug::Alert("Attempted To Create Sampler For Invalid Texture Type");
            break;
    }
}

void Texture::Clear() {
    ///Clean Texture
    vkDestroySampler(VulkanDeviceManager::GetSelectedDevice(), textureSampler, nullptr);
    vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), textureImageView, nullptr);
    vkDestroyImage(VulkanDeviceManager::GetSelectedDevice(), textureImage, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), textureImageMemory, nullptr);
    
    ///Clean HDR
    vkDestroySampler(VulkanDeviceManager::GetSelectedDevice(), hdrSampler, nullptr);
    vkDestroyImageView(VulkanDeviceManager::GetSelectedDevice(), hdrImageView, nullptr);
    vkDestroyImage(VulkanDeviceManager::GetSelectedDevice(), hdrImage, nullptr);
    vkFreeMemory(VulkanDeviceManager::GetSelectedDevice(), hdrImageMemory, nullptr);
}

void Texture::CleanHDRLoader() {

}