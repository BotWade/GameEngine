#include "RayDraw.hpp"
#include "../../Mesh/Mesh.hpp"
#include "../../Core/ShaderManager.hpp"
#include "../../Graphics/Vulkan/RenderPassManager.hpp"
#include "../../Graphics/Vulkan/PipelineManager.hpp"
#include "../../Graphics/Vulkan/VulkanDescriptor.hpp"
#include "../../Core/Camera.hpp"
#include "../../Graphics/Renderer.hpp"

#include <cstdlib>

void RayDraw::Load() {
    shader = ShaderManager::GetShader("Data/Shaders/Basic.shader");
    mesh = new Mesh();

    Vector3 data[2] = { ray.Origin, ray.GetPoint(50.0f) };

    mesh->UpdateData(&data, sizeof(Vector3) * 2);

    int indices[2] = { 0, 1};

    mesh->UpdateIndex(&indices, sizeof(int) * 2, 2);
    
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    uniformData = new VulkanDescriptor();
    uniformData->PrepareUniforms(shader);
    uniformData->GetUniform("MVP")->Generate(sizeof(Matrix4));
    uniformData->Generate(shader);

    pipeLine = PipelineManager::GetPipeline("RayDraw");

    if (pipeLine == nullptr) {
        pipeLine = new VulkanPipeLine();
        pipeLine->PreparePipelineLayout();
        pipeLine->vkPipeLineData.InputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        pipeLine->SetVertexInputInfo(Vector3::GetBindingDescription(), Vector3::GetAttributeDescriptions());
        pipeLine->CreatePipelineLayout(uniformData);
        pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
        pipeLine->Name = "RayDraw";

        PipelineManager::AddPipeline(pipeLine);
    }   
}

void RayDraw::Update() {
    model = Camera::ProjectionView * object->transform.ModelMatrix();
    uniformData->GetUniform("MVP")->UpdateUniform(&model);
}

void RayDraw::PreRender() {
    commandBuffer = CommandBufferManager::GetOrCreate("Ray - " + to_string(rand()));
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

void RayDraw::Render() {
    CommandBufferManager::AddToQueuePool(commandBuffer);
}

void RayDraw::Clean() {
    uniformData->Clean();
    mesh->Clean();
}