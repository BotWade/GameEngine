#ifndef _VULKANOFFSCREENMANAGER_
#define _VULKANOFFSCREENMANAGER_

#include "VulkanOffScreenRenderer.hpp"

class VulkanOffScreenManager {
public:
    static vector<VulkanOffScreenRenderer*> offScreenRendereres;

    static VulkanOffScreenRenderer* GetOffScreenRenderer(string Name);
    static void Clean();
};

#endif 