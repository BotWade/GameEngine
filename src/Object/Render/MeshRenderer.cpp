#include "MeshRenderer.hpp"
#include "../../Graphics/Vulkan/VulkanDescriptor.hpp"
#include "../../Graphics/Vulkan/RenderPassManager.hpp"
#include "../../Graphics/Vulkan/PipelineManager.hpp"
#include "../../Graphics/Renderer.hpp"
#include "../../Core/ShaderManager.hpp"
#include "../../Core/MeshManager.hpp"
#include "../../Core/TextureManager.hpp"
#include "../../Core/Camera.hpp"
#include "../../Math/Matrix4.hpp"

void MeshRenderer::PrepareObject(const char* Shader, const char* Model, Texture* Texture) {
    
    shader = ShaderManager::GetShader(Shader);
    mesh = MeshManager::GetMesh(Model);
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    uniformData = new VulkanDescriptor();
    uniformData->PrepareUniforms(shader);

    texture = Texture;
}

void MeshRenderer::LoadObject(string UniformName) {
    uniformData->Generate(shader);

    pipeLine = PipelineManager::GetPipeline("Vertex_" + UniformName);

    if (pipeLine == nullptr) {
        pipeLine = new VulkanPipeLine();
        pipeLine->PreparePipelineLayout();
        pipeLine->SetVertexInputInfo(Vertex::GetBindingDescription(), Vertex::GetAttributeDescriptions());
        pipeLine->CreatePipelineLayout(uniformData);
        pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
        pipeLine->Name = "Vertex_" + UniformName;

        PipelineManager::AddPipeline(pipeLine);
    }
}

void MeshRenderer::Update() {

    Matrix4 model = Camera::ProjectionView * object->transform.ModelMatrix();
    uniformData->GetUniform("MVP")->UpdateUniform(&model);
}

void MeshRenderer::PreRender() {
    commandBuffer = CommandBufferManager::Create();
    Vulkan::commandBuffer = commandBuffer;
    for (size_t Index = 0; Index < commandBuffer->commandBuffers.size(); Index++) {
        Vulkan::CurrentCommandBuffer = Index;

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(Vulkan::GetCurrentCommandBuffer(), &beginInfo) != VK_SUCCESS)
            Debug::Error("failed to begin recording command buffer!");

        Renderer::Prepare();

        pipeLine->Bind();
        mesh->Bind();
        uniformData->Bind(pipeLine);
        mesh->Draw();
        
        vkCmdEndRenderPass(Vulkan::GetCurrentCommandBuffer());

        if (vkEndCommandBuffer(Vulkan::GetCurrentCommandBuffer()) != VK_SUCCESS)
            Debug::Error("failed to record command buffer!");
    }
}

void MeshRenderer::Render() {
    CommandBufferManager::AddToQueuePool(commandBuffer);
}

void MeshRenderer::Clean() {
    uniformData->Clean();
}