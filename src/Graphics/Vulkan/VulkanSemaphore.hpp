#ifndef _VULKANSEMAPHORE_
#define _VULKANSEMAPHORE_

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

class VulkanSemaphore {
public:
    static vector<VkSemaphore> imageAvailableSemaphores;
    static vector<VkSemaphore> renderFinishedSemaphores;
    static vector<VkFence> inFlightFences;
    static vector<VkFence> imagesInFlight;
    
    static void CreateSemaphores();
    static void Clean();
};

#endif