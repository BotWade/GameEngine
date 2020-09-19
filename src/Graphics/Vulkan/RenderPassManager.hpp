#ifndef _RENDERPASSMANAGER_
#define _RENDERPASSMANAGER_

#include "VulkanRenderPass.hpp"
#include <vector>

using namespace std;

class RenderPassManager {
public:
    static vector<VulkanRenderPass*> RenderPasses;

    static void AddRenderPass(VulkanRenderPass* renderPass);
    static VulkanRenderPass* GetRenderPass(string Name);
    static void ClearRenderPass();
    static void RecreateRenderPass();
};

#endif