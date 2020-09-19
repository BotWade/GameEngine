#include "Skybox.hpp"
#include "../Core/ShaderManager.hpp"
#include "../Core/MeshManager.hpp"
#include "../Graphics/Vulkan/PipelineManager.hpp"
#include "../Core/TextureManager.hpp"
#include "../Graphics/Vulkan/RenderPassManager.hpp"
#include "../Core/Camera.hpp"
#include "../Graphics/Renderer.hpp"
#include "../Math/Matrix3.hpp"

void Skybox::LoadSkybox(const char* Skybox, const char* Mesh, const char* Shader) {
    shader = ShaderManager::GetShader(Shader);
    mesh = MeshManager::GetMesh(Mesh);

    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    uniformData.PrepareUniforms(shader);
    uniformData.GetUniform("VP")->Generate(sizeof(Matrix4));
    uniformData.GetUniform("skybox")->SetTexture(TextureManager::GetTexture(Skybox, HDR));
    uniformData.Generate(shader);

    pipeLine = new VulkanPipeLine();
    pipeLine->PreparePipelineLayout();
    pipeLine->vkPipeLineData.DepthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    pipeLine->vkPipeLineData.Rasterizer.cullMode = VK_CULL_MODE_NONE;
    pipeLine->SetVertexInputInfo(Vertex::GetBindingDescription(), Vertex::GetAttributeDescriptions());
    pipeLine->CreatePipelineLayout(&uniformData);
    pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
    pipeLine->Name = "BasicPipeline";

    PipelineManager::AddPipeline(pipeLine);
}

void Skybox::PreRender() {
    commandBuffer = CommandBufferManager::GetOrCreate(object->Tag);
    Vulkan::commandBuffer = commandBuffer;
    for (size_t Index = 0; Index < commandBuffer->commandBuffers.size(); Index++) {
        Vulkan::CurrentCommandBuffer = Index;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
            Debug::Error("failed to begin recording command buffer!");

        Renderer::Prepare();

        vkCmdBindPipeline(Vulkan::GetCurrentCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine->graphicsPipeline);

        mesh->Bind();
        uniformData.Bind(pipeLine);
        mesh->Draw();
        
        vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

        if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
            Debug::Error("failed to record command buffer!");
    }
}

void Skybox::Update() { 
    model = Camera::Projection * Matrix4(Matrix3(Camera::View));
}

void Skybox::PosRender() { 
    CommandBufferManager::AddToQueuePool(commandBuffer);
    uniformData.GetUniform("VP")->UpdateUniform(&model);
}

void Skybox::Clean() {
    uniformData.Clean();
}