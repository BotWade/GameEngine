#include "InterfaceObject.hpp"
#include "InterfaceManager.hpp"
#include "../Core/ShaderManager.hpp"
#include "../Graphics/Renderer.hpp"
#include "../Graphics/Vulkan/VulkanDescriptor.hpp"
#include "../Graphics/Vulkan/PipelineManager.hpp"
#include "../Graphics/Vulkan/RenderPassManager.hpp"
#include "../Graphics/Vulkan/CommandBufferManager.hpp"
#include "../Graphics/Shader.hpp"
#include "../Mesh/Mesh.hpp"
#include <algorithm>

InterfaceObject::InterfaceObject() {

    xConstraint = yConstraint = wConstraint = hConstraint = PIXEL;
    BackgroundColor = Vector4(1, 1, 1, 1);
    ForegroundColor = Vector4(0, 0, 0, 1);
    Position = Vector2(100, 100);
    Size = Vector2(100, 100);
    Active = true;
    Visibel = true;
    shader = nullptr;

    Children = vector<InterfaceObject*>();
    OnClick.F = OnHover.F = nullptr;
}

void InterfaceObject::Load() {

    if (shader == nullptr)
        LoadObject("Data/Shaders/Interface.shader");

    mesh = new Mesh();
    int indices[] = {0,2,1,
                     0,3,2};

    mesh->UpdateIndex(indices, sizeof(int) * 6, 6);
    if (!mesh->AttributesProcessed)
        mesh->ProcessAttributes(GL_FLOAT, sizeof(float), shader);

    mesh->AlocateData(sizeof(float) * 8);

    uniformData = new VulkanDescriptor();
    uniformData->PrepareUniforms(shader);
    uniformData->GetUniform("InterfaceData")->Generate(sizeof(InterfaceUniformData));
    uniformData->Generate(shader);

    pipeLine = PipelineManager::GetPipeline("Interface");
    
    if (pipeLine == nullptr) {
        pipeLine = new VulkanPipeLine();
        pipeLine->PreparePipelineLayout();
        pipeLine->vkPipeLineData.DepthStencil.depthTestEnable = VK_FALSE;
        pipeLine->SetVertexInputInfo(Vector2::GetBindingDescription(), Vector2::GetAttributeDescriptions());
        pipeLine->CreatePipelineLayout(uniformData);
        pipeLine->CreatePipeline(shader, RenderPassManager::GetRenderPass("Default"));
        pipeLine->Name = "Interface";

        PipelineManager::AddPipeline(pipeLine);
    }

    ExecuteChildren(LOAD);
}

void InterfaceObject::LoadObject(const char* Shader) {

    shader = ShaderManager::GetShader(Shader);
}

void InterfaceObject::AddChild(InterfaceObject* object) {

    object->Parent = this;
    Children.push_back(object);
}

void InterfaceObject::UpdatePosition() {
    if (xConstraint == PIXEL && yConstraint == PIXEL && wConstraint == PIXEL && hConstraint == PIXEL)
        RenderPosition = Vector4(Position, Size);
    else  {
        ///Process Size
        if (wConstraint == RELATIVE)
            RenderPosition.Z = Parent->Size.X / 100 * Size.X;
        if (hConstraint == RELATIVE)
            RenderPosition.W = Parent->Size.Y / 100 * Size.Y;

        if (wConstraint == PIXEL)
            RenderPosition.Z = Size.X;
        if (hConstraint == PIXEL)
            RenderPosition.W = Size.Y;

        if (wConstraint == ASPECT && hConstraint == ASPECT)
            RenderPosition.Z = RenderPosition.W = 10;
        else if (wConstraint == ASPECT)
            RenderPosition.Z = RenderPosition.W * Size.X;
        else if (hConstraint == ASPECT)
            RenderPosition.W = RenderPosition.Z * Size.Y;

        ///Process Position
        if (xConstraint == RELATIVE)
            RenderPosition.X = Parent->Size.X / 100 * Position.X;
        if (yConstraint == RELATIVE)
            RenderPosition.Y = Parent->Size.Y / 100 * Position.Y;

        if (xConstraint == PIXEL)
            RenderPosition.X = Position.X;
        if (yConstraint == PIXEL)
            RenderPosition.Y = Position.Y;

        if (xConstraint == ASPECT && yConstraint == ASPECT)
            RenderPosition.X = RenderPosition.Y = 10;
        else if (xConstraint == ASPECT)
            RenderPosition.X = RenderPosition.X * Position.X;
        else if (yConstraint == ASPECT)
            RenderPosition.Y = RenderPosition.Y * Position.Y;
    }

    RenderPosition.X += Parent->RenderPosition.X;
    RenderPosition.Y += Parent->RenderPosition.Y;

    if (Active && IsPointInsideRect(Input::MousePositionYInv, RenderPosition)) {
        if (OnHover.F != nullptr)
            OnHover.F(OnHover.Param);

        if (OnClick.F != nullptr && Input::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)) {
            OnClick.F(OnClick.Param);
            InterfaceManager::Interacted = true;
        }
    }
}

void InterfaceObject::UpdateMesh() {
    float vertices[4][2] = {{RenderPosition.X, RenderPosition.Y},
                            {RenderPosition.X, RenderPosition.Y + RenderPosition.W},
                            {RenderPosition.X + RenderPosition.Z, RenderPosition.Y + RenderPosition.W},
                            {RenderPosition.X + RenderPosition.Z, RenderPosition.Y} };

    if (mesh != nullptr)
        mesh->UpdateData(vertices, sizeof(float) * 8);
}

void InterfaceObject::Update() { 

    if (!Active)
        return;

    UpdatePosition();
    UpdateMesh();

    ExecuteChildren(UPDATE);
}

void InterfaceObject::PreRender() {
    commandBuffer = CommandBufferManager::GetOrCreate("Interface - " + Tag);
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
    
    ExecuteChildren(PRERENDER);
}

void InterfaceObject::Render() { 

    if (!Active)
        return;

    if (Visibel) {
        data.BackgroundColor = BackgroundColor;
        data.Projection = Window::OrthoProjection;
        uniformData->GetUniform("InterfaceData")->UpdateUniform(&data);

        CommandBufferManager::AddToQueuePool(commandBuffer);
    }

    ExecuteChildren(RENDER);
}

void InterfaceObject::PosRender() { 
    ExecuteChildren(POSRENDER);
}

void InterfaceObject::Clean() {
    if (uniformData != nullptr)
        uniformData->Clean();
    if (mesh != nullptr)
        mesh->Clean();

    ExecuteChildren(CLEAN);
}

void InterfaceObject::ExecuteChildren(ExecutionCode code) {
    size_t Size = Children.size();
    for (size_t Index = 0; Index < Size; Index++) {
        switch (code) {
            case LOAD:
                Children[Index]->Load();
                break;
            case UPDATE:
                Children[Index]->Update();
                break;
            case PRERENDER:
                Children[Index]->PreRender();
                break;
            case RENDER:
                Children[Index]->Render();
                break;
            case POSRENDER:
                Children[Index]->PosRender();
                break;
            case CLEAN:
                Children[Index]->Clean();
                break;
        }
    }
}