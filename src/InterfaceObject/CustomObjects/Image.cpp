#include "Image.hpp"
#include "../../Core/Window.hpp"
#include "../../Graphics/Vulkan/VulkanDescriptor.hpp"
#include "../../Graphics/Vulkan/PipelineManager.hpp"
#include "../../Graphics/Vulkan/RenderPassManager.hpp"
#include "../../Graphics/Vulkan/CommandBufferManager.hpp"
#include "../../Graphics/Shader.hpp"
#include "../../Graphics/Renderer.hpp"
#include "../../Mesh/Mesh.hpp"

void Image::Load() {
    if (shader == nullptr)
        LoadObject("Data/Shaders/InterfaceImage.shader");

    mesh = new Mesh();
    int indices[] = {0,2,1,
                     0,3,2};

    mesh->UpdateIndex(indices, sizeof(int) * 6, 6);
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    mesh->AlocateData(sizeof(float) * 16);

    uniformData = new VulkanDescriptor();
    uniformData->PrepareUniforms(shader);
    uniformData->GetUniform("projection")->Generate(sizeof(Matrix4));
    uniformData->GetUniform("texture")->SetTexture(texture);
    uniformData->Generate(shader);

    pipeLine = PipelineManager::GetPipeline("InterfaceImage");
    
    if (pipeLine == nullptr) {
        pipeLine = new VulkanPipeLine();
        pipeLine->PreparePipelineLayout();
        pipeLine->vkPipeLineData.DepthStencil.depthTestEnable = VK_FALSE;
        pipeLine->SetVertexInputInfo(Vector4::GetBindingDescription(), Vector4::GetAttributeDescriptions());
        pipeLine->CreatePipelineLayout(uniformData);
        pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
        pipeLine->Name = "InterfaceImage";

        PipelineManager::AddPipeline(pipeLine);
    }

    size_t Size = Children.size();
    for (size_t Index = 0; Index < Size; Index++)
        Children[Index]->Load();
}

void Image::Update() {
    UpdatePosition();

    float vertices[4][4] = {{RenderPosition.X, RenderPosition.Y                                         , 0.0f, 0.0f},
                            {RenderPosition.X, RenderPosition.Y + RenderPosition.W                      , 0.0f, 1.0f},
                            {RenderPosition.X + RenderPosition.Z, RenderPosition.Y + RenderPosition.W   , 1.0f, 1.0f},
                            {RenderPosition.X + RenderPosition.Z, RenderPosition.Y                      , 1.0f, 0.0f}};

    mesh->UpdateData(vertices, sizeof(float) * 16);

    ExecuteChildren(UPDATE);
}

void Image::Render() {
    if (!Active)
        return;

    if (Visibel) {
        uniformData->GetUniform("projection")->UpdateUniform(&Window::OrthoProjection);

        CommandBufferManager::AddToQueuePool(commandBuffer);
    }

    size_t Size = Children.size();
    for (size_t Index = 0; Index < Size; Index++)
        Children[Index]->Render();
}