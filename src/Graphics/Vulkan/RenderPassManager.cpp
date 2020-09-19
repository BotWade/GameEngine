#include "RenderPassManager.hpp"

vector<VulkanRenderPass*> RenderPassManager::RenderPasses;

void RenderPassManager::AddRenderPass(VulkanRenderPass* renderPass) {

    RenderPasses.push_back(renderPass);
}

VulkanRenderPass* RenderPassManager::GetRenderPass(string Name) {
    for (VulkanRenderPass* renderPass : RenderPasses)
        if (renderPass->Name == Name)
            return renderPass;

    return nullptr;
}

void RenderPassManager::ClearRenderPass() {

    for (VulkanRenderPass* renderPass : RenderPasses)
        renderPass->Clean();
}


void RenderPassManager::RecreateRenderPass() {

    for (VulkanRenderPass* renderPass : RenderPasses)
        renderPass->CreateRenderPass();
}