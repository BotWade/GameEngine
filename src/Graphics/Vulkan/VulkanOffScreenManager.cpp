#include "VulkanOffScreenManager.hpp"

vector<VulkanOffScreenRenderer*> VulkanOffScreenManager::offScreenRendereres;

VulkanOffScreenRenderer* VulkanOffScreenManager::GetOffScreenRenderer(string Name) {
    for (VulkanOffScreenRenderer* renderer : offScreenRendereres)
        if (renderer->Name == Name)
            return renderer;

    VulkanOffScreenRenderer* renderer = new VulkanOffScreenRenderer();
    renderer->Name = Name;
    offScreenRendereres.push_back(renderer);
    return renderer;
}

void VulkanOffScreenManager::Clean() {
    for (VulkanOffScreenRenderer* renderer : offScreenRendereres)
        renderer->Clean();
}